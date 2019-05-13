/*
 sd eh7 * hardware.h
 *
 *  Created on: Feb 28, 2019
 *      Author: Zandwich
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_tim.h"

#include "stm32f4timer.hpp"

//*************************SPI****************************************
//-------------------------SPI1----------------------
#define SPI1_ALTERNATE_FUNCTION		GPIO_AF_SPI1

#define SPI1_MOSI_GPIO				GPIOA
#define SPI1_MOSI_PinSource			GPIO_PinSource7
#define SPI1_MOSI_Pin				GPIO_Pin_7

#define SPI1_CLK_GPIO				GPIOA
#define SPI1_CLK_PinSource			GPIO_PinSource5
#define SPI1_CLK_Pin				GPIO_Pin_5
//-------------------------SPI2----------------------
#define SPI2_ALTERNATE_FUNCTION		GPIO_AF_SPI2

#define SPI2_MOSI_GPIO				GPIOC
#define SPI2_MOSI_PinSource			GPIO_PinSource3
#define SPI2_MOSI_Pin				GPIO_Pin_3

#define SPI2_CLK_GPIO				GPIOB
#define SPI2_CLK_PinSource			GPIO_PinSource10
#define SPI2_CLK_Pin				GPIO_Pin_10
//-------------------------SPI3----------------------
#define SPI3_CLK_ALTERNATE_FUNCTION		0x07
#define SPI3_MOSI_ALTERNATE_FUNCTION		0x06

#define SPI3_MOSI_GPIO				GPIOC
#define SPI3_MOSI_PinSource			GPIO_PinSource12
#define SPI3_MOSI_Pin				GPIO_Pin_12

#define SPI3_CLK_GPIO				GPIOB
#define SPI3_CLK_PinSource			GPIO_PinSource12
#define SPI3_CLK_Pin				GPIO_Pin_12
//-------------------------SPI4----------------------
#define SPI4_MOSI_ALTERNATE_FUNCTION		0x05
#define SPI4_CLK_ALTERNATE_FUNCTION			0x06

#define SPI4_MOSI_GPIO				GPIOA
#define SPI4_MOSI_PinSource			GPIO_PinSource1
#define SPI4_MOSI_Pin				GPIO_Pin_1

#define SPI4_CLK_GPIO				GPIOB
#define SPI4_CLK_PinSource			GPIO_PinSource13
#define SPI4_CLK_Pin				GPIO_Pin_13

//-------------------------SPI5----------------------

#define SPI5_ALTERNATE_FUNCTION		0x06

#define SPI5_MOSI_GPIO				GPIOA
#define SPI5_MOSI_PinSource			GPIO_PinSource10
#define SPI5_MOSI_Pin				GPIO_Pin_10

#define SPI5_MISO_GPIO				GPIOA
#define SPI5_MISO_PinSource			GPIO_PinSource12
#define SPI5_MISO_Pin				GPIO_Pin_12

#define SPI5_CLK_GPIO				GPIOB
#define SPI5_CLK_PinSource			GPIO_PinSource0
#define SPI5_CLK_Pin				GPIO_Pin_0

#define SPI5_NSS_GPIO				GPIOB
#define SPI5_NSS_PinSource			GPIO_PinSource1
#define SPI5_NSS_Pin				GPIO_Pin_1

//*************************TLC5955*****************************
#define TLC_LAT1_GPIO				GPIOA
#define	TLC_LAT1_Pin				GPIO_Pin_15
#define TLC_LAT1_PinSource			GPIO_PinSource15

#define TLC_LAT2_GPIO				GPIOC
#define TLC_LAT2_Pin				GPIO_Pin_10
#define TLC_LAT2_PinSource			GPIO_PinSource10

#define GSCLK_GPIO					GPIOA
#define GSCLK_Pin					GPIO_Pin_0
#define GSCLK_PinSource				GPIO_PinSource0

//*************************OTHER*****************************
#define PULSE_GPIO					GPIOB
#define PULSE_Pin					GPIO_Pin_2
#define PULSE_PinSource				GPIO_PinSource2
//***********************FREQUENCY***********************************
#define GSCLK_FREQ					1000000 //1MHZ

//**************************USART1*****************************
#define USART1_ALTERNATE_FUNCTION	GPIO_AF_USART1

#define USART1_TX_GPIO				GPIOA
#define USART1_TX_PinSource			GPIO_PinSource9
#define USART1_TX_Pin				GPIO_Pin_9
#define USART1_RX_GPIO				GPIOB
#define USART1_RX_PinSource			GPIO_PinSource7
#define USART1_RX_Pin				GPIO_Pin_7

//**************************USART2*****************************
#define USART2_ALTERNATE_FUNCTION	GPIO_AF_USART2

#define USART2_TX_GPIO				GPIOA
#define USART2_TX_PinSource			GPIO_PinSource2
#define USART2_TX_Pin				GPIO_Pin_2
#define USART2_RX_GPIO				GPIOA
#define USART2_RX_PinSource			GPIO_PinSource3
#define USART2_RX_Pin				GPIO_Pin_3

//*************GLOBAL VARIABLEs********************************
//static uint8_t screenBuffer1[289];
//static uint8_t screenBuffer2[289];
//static uint8_t screenBuffer3[289];
//static uint8_t screenBuffer4[289];
//static uint8_t screenBuffer5[289];

//******************GLOBAL FUNCTIONS***************************
void initGPIO();
void initDMA();
void initRCC();
void initEXTI();
void initNVIC();
void initTIM();

#endif /* HARDWARE_H_ */
