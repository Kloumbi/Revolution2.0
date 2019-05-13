/**
 ******************************************************************************
 * @file    main.c

 * @author  MadChar
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "stdint-gcc.h"

#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <string.h>

#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_tim.h"

#include "hardware.h"
#include "LEDMAP.h"
#include "tlc5955.hpp"
#include "stm32spi1.hpp"
#include "stm32spi2.hpp"
#include "stm32spi3.hpp"
#include "stm32spi4.hpp"
#include "stm32f4timer.hpp"
#include "stm32f411USART1.hpp"
#include "stm32f411USART2.hpp"
#include "flash.hpp"


//uint8_t bufferSpiTx[1156];
//uint8_t spi1[289];
//uint8_t spi2[289];
//uint8_t spi3[289];
//uint8_t spi4[289];
//Buffer<uint8_t, 1024> b;



uint8_t bufferSpiTx[1156];
uint8_t bufferSpiRx[1156];

constexpr uint8_t PIXELMAP[] = PIXEL_ARRAY;

bool flagRefreshBuffer = false;
bool displayOffLatch = false;

bool flagDMA_TX_Complete1 = false;
bool flagDMA_TX_Complete2 = false;
bool flagDMA_TX_Complete3 = false;
bool flagDMA_TX_Complete4 = false;

static constexpr uint8_t blackRow[289] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static constexpr uint8_t pixelMap[3][289] =
		{ { 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03,
				0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00,
				0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC,
				0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03,
				0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00,
				0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC,
				0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03,
				0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00,
				0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC,
				0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x01,
				0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00,
				0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE,
				0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01,
				0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00,
				0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE,
				0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01,
				0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00,
				0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE,
				0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01,
				0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
				0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
				0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
				0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
				0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
				0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
				0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
				0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
				0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
				0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF }, { 0x03, 0xFF,
				0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00,
				0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00,
				0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF,
				0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00,
				0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00,
				0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF,
				0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00,
				0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00,
				0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF,
				0xFC, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00,
				0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00,
				0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF,
				0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00,
				0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00,
				0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF,
				0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00,
				0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00,
				0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF,
				0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00,
				0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00,
				0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF,
				0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
				0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00,
				0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF,
				0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
				0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00,
				0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF,
				0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
				0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x03,
				0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00,
				0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC,
				0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03,
				0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00,
				0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC,
				0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03,
				0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00,
				0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC,
				0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x03,
				0xFF, 0xFC, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00,
				0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE,
				0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01,
				0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00,
				0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE,
				0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01,
				0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00,
				0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE,
				0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01,
				0xFF, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFE, 0x00, 0x00,
				0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
				0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
				0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
				0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
				0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
				0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
				0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
				0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
				0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
				0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00 } };

static uint16_t compteurTest = 0;
static uint8_t testCount = 0;
static uint16_t columnCounter = 0;
static uint8_t imageNum = 0;
static uint8_t z = 0;

static bool debug = true;

enum interlace_e {
	DISPLAY_ODD_SIDE, DISPLAY_EVEN_SIDE
} interlacing = DISPLAY_ODD_SIDE;
enum display_e {
	ON, OFF
} displayState = OFF;
enum bufferIndex_e {
	BUFFER_ODD, BUFFER_EVEN
} bufferIndex = BUFFER_ODD;

//static uint8_t pixelMapOddBuffer1[289];
//static uint8_t pixelMapOddBuffer2[289];
//static uint8_t pixelMapOddBuffer3[289];
//static uint8_t pixelMapOddBuffer4[289];
//
//static uint8_t pixelMapEvenBuffer1[289];
//static uint8_t pixelMapEvenBuffer2[289];
//static uint8_t pixelMapEvenBuffer3[289];
//static uint8_t pixelMapEvenBuffer4[289];

//extern "C" void SysTick_Handler(void);
extern "C" void DMA2_Stream2_IRQHandler(void);
extern "C" void DMA1_Stream4_IRQHandler(void);
extern "C" void DMA1_Stream5_IRQHandler(void);
extern "C" void DMA2_Stream1_IRQHandler(void);
extern "C" void TIM4_IRQHandler(void);
extern "C" void EXTI2_IRQHandler(void);

void DMA2_Stream2_IRQHandler(void) {
	if (DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2) == SET) {
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		flagDMA_TX_Complete1 = true;
		DMA2_Stream2->CR &= ~(uint32_t) DMA_SxCR_EN;

		if (flagDMA_TX_Complete1 && flagDMA_TX_Complete2 && flagDMA_TX_Complete3
				&& flagDMA_TX_Complete4) {
			if (interlacing == DISPLAY_ODD_SIDE) {
				TLC_LAT1_GPIO->BSRRH = TLC_LAT1_Pin;
				TLC_LAT1_GPIO->BSRRL = TLC_LAT1_Pin;
				TLC_LAT1_GPIO->BSRRH = TLC_LAT1_Pin;
			} else {
				TLC_LAT2_GPIO->BSRRH = TLC_LAT2_Pin;
				TLC_LAT2_GPIO->BSRRL = TLC_LAT2_Pin;
				TLC_LAT2_GPIO->BSRRH = TLC_LAT2_Pin;
			}

			if (displayOffLatch) {
				if (interlacing == DISPLAY_EVEN_SIDE) {
					TLC_LAT1_GPIO->BSRRH = TLC_LAT1_Pin;
					TLC_LAT1_GPIO->BSRRL = TLC_LAT1_Pin;
					TLC_LAT1_GPIO->BSRRH = TLC_LAT1_Pin;
				} else {
					TLC_LAT2_GPIO->BSRRH = TLC_LAT2_Pin;
					TLC_LAT2_GPIO->BSRRL = TLC_LAT2_Pin;
					TLC_LAT2_GPIO->BSRRH = TLC_LAT2_Pin;
				}
				displayOffLatch = false;
			}
			flagDMA_TX_Complete1 = false;
			flagDMA_TX_Complete2 = false;
			flagDMA_TX_Complete3 = false;
			flagDMA_TX_Complete4 = false;
		}

	}
}

void DMA1_Stream4_IRQHandler(void) {
	if (DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4) == SET) {
		DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
		DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
		flagDMA_TX_Complete2 = true;
		DMA1_Stream4->CR &= ~(uint32_t) DMA_SxCR_EN;

		if (flagDMA_TX_Complete1 && flagDMA_TX_Complete2 && flagDMA_TX_Complete3
				&& flagDMA_TX_Complete4) {
			if (interlacing == DISPLAY_ODD_SIDE) {

				TLC_LAT1_GPIO->BSRRH = TLC_LAT1_Pin;
				TLC_LAT1_GPIO->BSRRL = TLC_LAT1_Pin;
				TLC_LAT1_GPIO->BSRRH = TLC_LAT1_Pin;
			} else {
				TLC_LAT2_GPIO->BSRRH = TLC_LAT2_Pin;
				TLC_LAT2_GPIO->BSRRL = TLC_LAT2_Pin;
				TLC_LAT2_GPIO->BSRRH = TLC_LAT2_Pin;
			}

			flagDMA_TX_Complete1 = false;
			flagDMA_TX_Complete2 = false;
			flagDMA_TX_Complete3 = false;
			flagDMA_TX_Complete4 = false;
		}
	}
}

void DMA1_Stream5_IRQHandler(void) {
	if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5) == SET) {
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
		flagDMA_TX_Complete3 = true;
		DMA1_Stream5->CR &= ~(uint32_t) DMA_SxCR_EN;

		if (flagDMA_TX_Complete1 && flagDMA_TX_Complete2 && flagDMA_TX_Complete3
				&& flagDMA_TX_Complete4) {
			if (interlacing == DISPLAY_ODD_SIDE) {
				TLC_LAT1_GPIO->BSRRH = TLC_LAT1_Pin;
				TLC_LAT1_GPIO->BSRRL = TLC_LAT1_Pin;
				TLC_LAT1_GPIO->BSRRH = TLC_LAT1_Pin;
			} else {
				TLC_LAT2_GPIO->BSRRH = TLC_LAT2_Pin;
				TLC_LAT2_GPIO->BSRRL = TLC_LAT2_Pin;
				TLC_LAT2_GPIO->BSRRH = TLC_LAT2_Pin;
			}

			flagDMA_TX_Complete1 = false;
			flagDMA_TX_Complete2 = false;
			flagDMA_TX_Complete3 = false;
			flagDMA_TX_Complete4 = false;
		}

	}
}

void DMA2_Stream1_IRQHandler(void) {
	if (DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1) == SET) {
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
		DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
		flagDMA_TX_Complete4 = true;
		DMA2_Stream1->CR &= ~(uint32_t) DMA_SxCR_EN;

		if (flagDMA_TX_Complete1 && flagDMA_TX_Complete2 && flagDMA_TX_Complete3
				&& flagDMA_TX_Complete4) {
			if (interlacing == DISPLAY_ODD_SIDE) {
				TLC_LAT1_GPIO->BSRRH = TLC_LAT1_Pin;
				TLC_LAT1_GPIO->BSRRL = TLC_LAT1_Pin;
				TLC_LAT1_GPIO->BSRRH = TLC_LAT1_Pin;
			} else {
				TLC_LAT2_GPIO->BSRRH = TLC_LAT2_Pin;
				TLC_LAT2_GPIO->BSRRL = TLC_LAT2_Pin;
				TLC_LAT2_GPIO->BSRRH = TLC_LAT2_Pin;
			}

			flagDMA_TX_Complete1 = false;
			flagDMA_TX_Complete2 = false;
			flagDMA_TX_Complete3 = false;
			flagDMA_TX_Complete4 = false;
		}
	}
}

void TIM4_IRQHandler(void) {
	if (TIM_GetFlagStatus(TIM4, TIM_IT_Update) != RESET) {
		//Clear flag Interrupt
		TIM4->SR = (uint16_t) ~TIM_IT_Update;
		flagRefreshBuffer = true;
		columnCounter++;

		if (columnCounter < 128)
			z = 0;
		else
			z = 1;
		if (columnCounter == 254)
			displayState = OFF;
		if (columnCounter == 255) {
			if (interlacing == DISPLAY_ODD_SIDE) {
				interlacing = DISPLAY_EVEN_SIDE;
				displayState = ON;
			} else {
				interlacing = DISPLAY_ODD_SIDE;
				displayState = OFF;
			}
			columnCounter = 0;
		}

		//Disable DMA
		DMA2_Stream2->CR &= ~(uint32_t) DMA_SxCR_EN;
		DMA1_Stream4->CR &= ~(uint32_t) DMA_SxCR_EN;
		DMA1_Stream5->CR &= ~(uint32_t) DMA_SxCR_EN;
		DMA2_Stream1->CR &= ~(uint32_t) DMA_SxCR_EN;

		switch (interlacing) {
		case DISPLAY_ODD_SIDE:
			switch (displayState) {
			case ON:
				switch (bufferIndex) {
				case BUFFER_ODD:
					DMA2_Stream2->M0AR = (uint32_t) &pixelMap[z];
					DMA1_Stream4->M0AR = (uint32_t) &pixelMap[z];
					DMA1_Stream5->M0AR = (uint32_t) &pixelMap[z];
					DMA2_Stream1->M0AR = (uint32_t) &pixelMap[z];
					break;
				case BUFFER_EVEN:
					DMA2_Stream2->M0AR = (uint32_t) &pixelMap[z];
					DMA1_Stream4->M0AR = (uint32_t) &pixelMap[z];
					DMA1_Stream5->M0AR = (uint32_t) &pixelMap[z];
					DMA2_Stream1->M0AR = (uint32_t) &pixelMap[z];
					break;
				}
				if (bufferIndex == BUFFER_EVEN)
					bufferIndex = BUFFER_ODD;
				else
					bufferIndex = BUFFER_EVEN;
				break;
			case OFF:
				DMA2_Stream2->M0AR = (uint32_t) &blackRow;
				DMA1_Stream4->M0AR = (uint32_t) &blackRow;
				DMA1_Stream5->M0AR = (uint32_t) &blackRow;
				DMA2_Stream1->M0AR = (uint32_t) &blackRow;
				break;
			}
			break;
		case DISPLAY_EVEN_SIDE:
			switch (displayState) {
			case ON:
				switch (bufferIndex) {
				case BUFFER_EVEN:
					DMA2_Stream2->M0AR = (uint32_t) &pixelMap[z];
					DMA1_Stream4->M0AR = (uint32_t) &pixelMap[z];
					DMA1_Stream5->M0AR = (uint32_t) &pixelMap[z];
					DMA2_Stream1->M0AR = (uint32_t) &pixelMap[z];
					break;
				case BUFFER_ODD:
					DMA2_Stream2->M0AR = (uint32_t) &pixelMap[z];
					DMA1_Stream4->M0AR = (uint32_t) &pixelMap[z];
					DMA1_Stream5->M0AR = (uint32_t) &pixelMap[z];
					DMA2_Stream1->M0AR = (uint32_t) &pixelMap[z];
					break;
				}
				break;
			case OFF:
				DMA2_Stream2->M0AR = (uint32_t) &blackRow;
				DMA1_Stream4->M0AR = (uint32_t) &blackRow;
				DMA1_Stream5->M0AR = (uint32_t) &blackRow;
				DMA2_Stream1->M0AR = (uint32_t) &blackRow;
				break;
			}
		}

		if (bufferIndex == BUFFER_EVEN)
			bufferIndex = BUFFER_ODD;
		else
			bufferIndex = BUFFER_EVEN;
		//if(displayState==ON) displayState = OFF;
		//else displayState = ON;

		if (displayState == OFF)
			displayState = ON;

		//Start DMA2 Stream2
		SPI1->CR2 |= SPI_I2S_DMAReq_Tx;
		DMA2_Stream2->CR |= (uint32_t) DMA_SxCR_EN;

		//Start DMA1 Stream4
		SPI2->CR2 |= SPI_I2S_DMAReq_Tx;
		DMA1_Stream4->CR |= (uint32_t) DMA_SxCR_EN;

		//Start DMA1 Stream5
		SPI3->CR2 |= SPI_I2S_DMAReq_Tx;
		DMA1_Stream5->CR |= (uint32_t) DMA_SxCR_EN;

		//Start DMA2 Stream1
		SPI4->CR2 |= SPI_I2S_DMAReq_Tx;
		DMA2_Stream1->CR |= (uint32_t) DMA_SxCR_EN;

	}
}

void EXTI2_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line2) != RESET) {

		columnCounter = 0;

		TIM4->CR1 &= ~TIM_CR1_CEN;
		TIM4->CR1 |= TIM_CR1_CEN;
		EXTI_ClearITPendingBit(EXTI_Line2);
	}

}

int main(void) {

	//----------------------------------RCC INIT-----------------------------------------------------
	//Enable SPIs clocks

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);

	//Enable DMA clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	//Enable GPIO clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	//Enable Timers clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//Enable SYSCFG clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	//----------------------------------GSCLOCK INIT-----------------------------------------------------

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GSCLK_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GSCLK_GPIO, &GPIO_InitStructure);

	GPIO_PinAFConfig(GSCLK_GPIO, GSCLK_PinSource, GPIO_AF_TIM2);

	GPIO_InitStructure.GPIO_Pin = PULSE_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(PULSE_GPIO, &GPIO_InitStructure);

	//---------------LATCH 1 INIT------------------------------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin = TLC_LAT1_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(TLC_LAT1_GPIO, &GPIO_InitStructure);
	//---------------LATCH 2 INIT------------------------------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin = TLC_LAT2_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(TLC_LAT2_GPIO, &GPIO_InitStructure);

	//----------------------------------COM-----------------------------------------------------------------------------
	STM32F411USART2 *console = STM32F411USART2::getInstance();

	console->sendString("\nConsole online\n\r");

	//----------------------------------SPI INIT-----------------------------------------------------
	if (debug)
		console->sendString("Initiating SPI...\n\r");
	STM32SPI1 spi1;
	STM32SPI2 spi2;
	STM32SPI3 spi3;
	STM32SPI4 spi4;

	spi1.init();
	spi2.init();
	spi3.init();
	spi4.init();

	if (debug)
		console->sendString("Done.\n\r");

	//----------------------------------TLC5955 INIT-----------------------------------------------------

	if (debug)
		console->sendString("Initiating TLC5955...\n\r");
	TLC5955 tlc;

	tlc.init(&spi1, &spi2, &spi3, &spi4);

	tlc.setRGBPinOrder(0, 1, 2);

	tlc.setAllDcData(127);
	tlc.setMaxCurrent(2, 2, 2);
	tlc.setFunctionControlData(true, true, true, true, true);
	tlc.setBrightnessCurrent(127, 5, 10);
	tlc.updateControl();

	if (debug)
		console->sendString("Done.\n\r");

	//----------------------------------DMA INIT-----------------------------------------------------
	if (debug)
		console->sendString("Initiating DMA...\n\r");
	DMA_InitTypeDef dma_spi;

	//DMA SPI1
	dma_spi.DMA_Channel = DMA_Channel_2;
	dma_spi.DMA_Memory0BaseAddr = (uint32_t) &pixelMap[0];
	dma_spi.DMA_PeripheralBaseAddr = (uint32_t) (&(SPI1->DR));
	dma_spi.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_spi.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
	dma_spi.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	dma_spi.DMA_Mode = DMA_Mode_Normal;
	dma_spi.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_spi.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_spi.DMA_BufferSize = sizeof(pixelMap[0]);
	dma_spi.DMA_Priority = DMA_Priority_High;
	dma_spi.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dma_spi.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	dma_spi.DMA_FIFOMode = DMA_FIFOMode_Disable;

	DMA_Init(DMA2_Stream2, &dma_spi);

	//DMA SPI2
	dma_spi.DMA_Channel = DMA_Channel_0;
	dma_spi.DMA_Memory0BaseAddr = (uint32_t) &pixelMap[0];
	dma_spi.DMA_PeripheralBaseAddr = (uint32_t) (&(SPI2->DR));
	dma_spi.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_spi.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
	dma_spi.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	dma_spi.DMA_Mode = DMA_Mode_Normal;
	dma_spi.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_spi.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_spi.DMA_BufferSize = sizeof(pixelMap[0]);
	dma_spi.DMA_Priority = DMA_Priority_High;
	dma_spi.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dma_spi.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	dma_spi.DMA_FIFOMode = DMA_FIFOMode_Disable;

	DMA_Init(DMA1_Stream4, &dma_spi);

	//DMA SPI3
	dma_spi.DMA_Channel = DMA_Channel_0;
	dma_spi.DMA_Memory0BaseAddr = (uint32_t) &pixelMap[0];
	dma_spi.DMA_PeripheralBaseAddr = (uint32_t) (&(SPI3->DR));
	dma_spi.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_spi.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
	dma_spi.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	dma_spi.DMA_Mode = DMA_Mode_Normal;
	dma_spi.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_spi.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_spi.DMA_BufferSize = sizeof(pixelMap[0]);
	dma_spi.DMA_Priority = DMA_Priority_High;
	dma_spi.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dma_spi.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	dma_spi.DMA_FIFOMode = DMA_FIFOMode_Disable;

	DMA_Init(DMA1_Stream5, &dma_spi);

	//DMA SPI4
	dma_spi.DMA_Channel = DMA_Channel_4;
	dma_spi.DMA_Memory0BaseAddr = (uint32_t) &pixelMap[0];
	dma_spi.DMA_PeripheralBaseAddr = (uint32_t) (&(SPI4->DR));
	dma_spi.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_spi.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
	dma_spi.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	dma_spi.DMA_Mode = DMA_Mode_Normal;
	dma_spi.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_spi.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_spi.DMA_BufferSize = sizeof(pixelMap[0]);
	dma_spi.DMA_Priority = DMA_Priority_High;
	dma_spi.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dma_spi.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	dma_spi.DMA_FIFOMode = DMA_FIFOMode_Disable;

	DMA_Init(DMA2_Stream1, &dma_spi);

	//Enable the transfer complete interrupt
	DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);

	if (debug)
		console->sendString("Done.\n\r");

	//----------------------------------External Interrupt INIT-----------------------------------------------------
	if (debug)
		console->sendString("Initiating EXTI...\n\r");
	EXTI_InitTypeDef EXTI_InitStruct;

	//PULSE PB2 for EXTI_Line2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);

	/* PB2 is connected to EXTI_Line2 */
	EXTI_InitStruct.EXTI_Line = EXTI_Line2;
	/* Enable interrupt */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	/* Interrupt mode */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	/* Triggers on rising and falling edge */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	/* Add to EXTI */
	EXTI_Init(&EXTI_InitStruct);
	if (debug)
		console->sendString("Done.\n\r");

	//----------------------------------Nested Vectored Interrupt Controller INIT-----------------------------------------------------
	if (debug)
		console->sendString("Initiating NVIC...\n\r");
	NVIC_InitTypeDef NVIC_InitStructure;

	//Enable the DMA2 Stream2 (SPI1_TX) Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(DMA2_Stream2_IRQn);

	//Enable the DMA1 Stream4 (SPI2_TX) Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(DMA1_Stream4_IRQn);

	//Enable the DMA1 Stream5 (SPI3_TX) Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(DMA1_Stream5_IRQn);

	//Enable the DMA2 Stream1 (SPI4_TX) Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(DMA2_Stream1_IRQn);

	// Enable Timer4 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable EXTI2 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	if (debug)
		console->sendString("Done.\n\r");

	//----------------------------------TIMER INIT-----------------------------------------------------
	if (debug)
		console->sendString("Starting timers...\n\r");
	STM32F4Timer gsclkTimer(TIM2, 1000000, 0, 0);
	gsclkTimer.enablePWM(1, 50);
	gsclkTimer.startTimer();

	//STM32F4Timer latchTimer(TIM4,50,65535,false);
	STM32F4Timer latchTimer(TIM4, 5973, 0, false);
	NVIC_EnableIRQ(TIM4_IRQn);
	latchTimer.enableITUpdate();
	latchTimer.startTimer();
	if (debug)
		console->sendString("Done.\n\r");
	for (int i = 0; i < 10000000; i++) {
		asm("nop");
	}

//	char car;
	uint8_t bufferConsole[1200];
//	bufferConsole[1156] = 0;

	//if(debug) console->sendString("Done.\n\r");
	//flash->getPixelColumn(0,0,pixelMapEvenBuffer1,pixelMapEvenBuffer2,pixelMapEvenBuffer3,pixelMapEvenBuffer4);
	//flash->getPixelColumn(0,0,pixelMapOddBuffer1,pixelMapOddBuffer2,pixelMapOddBuffer3,pixelMapOddBuffer4);

	if (debug)
		console->sendString("Starting DMA...\n\r");
	//Start DMA2 Stream2
	SPI1->CR2 |= SPI_I2S_DMAReq_Tx;
	DMA2_Stream2->CR |= (uint32_t) DMA_SxCR_EN;

	//Start DMA1 Stream4
	SPI2->CR2 |= SPI_I2S_DMAReq_Tx;
	DMA1_Stream4->CR |= (uint32_t) DMA_SxCR_EN;

	//Start DMA1 Stream5
	SPI3->CR2 |= SPI_I2S_DMAReq_Tx;
	DMA1_Stream5->CR |= (uint32_t) DMA_SxCR_EN;

	//Start DMA2 Stream1
	SPI4->CR2 |= SPI_I2S_DMAReq_Tx;
	DMA2_Stream1->CR |= (uint32_t) DMA_SxCR_EN;
	if (debug)
		console->sendString("Done.\n\r");
	if (debug)
		console->sendString("All systems nominal.\n\r");

	STM32F411USART1* com1 = STM32F411USART1::getInstance();
	com1->setEcho(false);
//	com1->sendString("\rRevolution v3.0\rCommunication : En ligne\r");

	Flash *flash = Flash::getInstance(false);
	flash->init();
	Flash::address_t add;
	add.page = 99;
	add.byte = 0;

//	com1->sendString("Next free saving slot : ");
//	com1->sendbyteToString(flash->getNextFreeImageSlot());
//	com1->sendString("\n\r");
//	com1->sendByte32ToBinaryString(flash->positionOfPresentImages);
	//	com1->sendString("\n\r");

	//flash->getPixelColumn(0, 0, spi1, spi2, spi3, spi4);

	//	uint8_t byte[] = "1234";
	//	flash->writeByte(&add, byte, 4);
	for (int i = 0; i < 14; i++) {
		//		flash->setFilename(i, "test.bmp");
		//		flash->resetFilename(i);
		//		flash->resetImageInCarrousel(i);
		//		com1->write(spi4[i]);
	}

	//flash->resetFilename(1);
	uint8_t c[4];
	c[0] = 'T';
	c[0] = 'e';
	c[0] = 's';
	c[0] = 't';
	//flash->setFilename(0, c);
	//flash->setFilename(4, "black_hole.bmp");
	uint8_t t;

//	for (int i = 0; i < 512; i++) {
//		t = flash->readByte(&add);
//		com1->write(t);
//		add.byte++;
//	}

//		com1->sendString("\n\r");
//		for (int i = 0; i < 289; i++) {
//			com1->write(spi1[i]);
//		}

	flash->getPixelColumnToString(0,0);


	while (1) {

		com1->incommingDataDecoder(flash);
		/*
		 if (com1->dataAvailable()) {
		 com1->write(com1->read());
		 }
		 */

//		if (console->dataAvailable()) {
//			car = console->read();
//			//console->write(car);
//			if (car == '1') {
//				int c = 0;
//
//				uint16_t cnt = 0;
//				console->sendString("Waiting for transmission #");
//				console->sendbyteToString(testCount);
//				console->sendString("\n\r");
//				while (c < 256) {
//					while (!console->dataAvailable())
//						;
//					console->sendString("Receiving transmission #");
//					console->sendbyteToString(testCount);
//					console->sendString("\n\r");
//					while (cnt < 1156) {
//						if (console->dataAvailable()) {
//							bufferConsole[cnt] = console->read();
//							cnt++;
//						}
//					}
//
//					if (debug)
//						console->sendString("Saving to flash...\n\r");
//					//flash->savePixelColumn(0,0,bufferConsole);
//					testCount++;
//					if (debug)
//						console->sendString("Success.\n\r");
//					if (testCount == 256)
//						testCount = 0;
//
//					//if (imageNum==Flash::MaxImageStored) imageNum = 0;
//					car = 0;
//					cnt = 0;
//					c++;
//				}
//				console->sendString("Transmission done.\n\r");
//			}
//		}
		if (flagRefreshBuffer) {
			//if (bufferIndexFlag==true) flash->getPixelColumn(10,columnCounter,pixelMapEvenBuffer1,pixelMapEvenBuffer2,pixelMapEvenBuffer3,pixelMapEvenBuffer4);
			//else flash->getPixelColumn(10,columnCounter,pixelMapOddBuffer1,pixelMapOddBuffer2,pixelMapOddBuffer3,pixelMapOddBuffer4);
			flagRefreshBuffer = false;
		}
	}
}

