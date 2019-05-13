/*
 * STM32F411USART2.cpp
 *
 *  Created on: Apr 24, 2019
 *      Author: rev
 */
#include "stm32f411USART2.hpp"
#include "stm32f4xx_rcc.h"
#include <string.h>
#include <cstdlib>

STM32F411USART2::STM32F411USART2() {
	// Activer les horloges
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	// Configurer IO pour RX et TX
	GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1; // PA2 et PA3 en Alternate fonction
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR2; // PA2 et PA3 en High speed
	GPIOA->AFR[0] |= GPIO_AF_USART2 << 8;   //PA2 en TX = AF7
	GPIOA->AFR[0] |= GPIO_AF_USART2 << 12;  //PA3 en RX = AF7

	SYSCFG->CMPCR |= SYSCFG_CMPCR_CMP_PD; // Active la compensation pour réduire le bruit.

	// Par default la configuration est 8N1
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE; // Active RX et TX
	setBaudRate(115200);

	NVIC_InitTypeDef NVIC_InitStructure;
	//Enable the USART2 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Active l'interruption pour USART2
	NVIC_EnableIRQ(USART2_IRQn);
	USART2->CR1 |= USART_CR1_RXNEIE; // Interruption de tx activée sur transmission

	USART2->CR1 |= USART_CR1_UE; // USART activé

}

STM32F411USART2::~STM32F411USART2() {
	if (instance)
		delete instance;
}

void STM32F411USART2::setBaudRate(uint32_t baudrate) {

	uint32_t tmpreg = 0x00, apbclock = 0x00;
	uint32_t integerdivider = 0x00;
	uint32_t fractionaldivider = 0x00;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	/*---------------------------- USART BRR Configuration -----------------------*/
	/* Configure the USART Baud Rate */
	RCC_GetClocksFreq(&RCC_ClocksStatus);

	apbclock = RCC_ClocksStatus.PCLK1_Frequency;

	/* Determine the integer part */
	if ((USART2->CR1 & USART_CR1_OVER8) != 0) {
		/* Integer part computing in case Oversampling mode is 8 Samples */
		integerdivider = ((25 * apbclock) / (2 * (baudrate)));
	} else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
	{
		/* Integer part computing in case Oversampling mode is 16 Samples */
		integerdivider = ((25 * apbclock) / (4 * (baudrate)));
	}
	tmpreg = (integerdivider / 100) << 4;

	/* Determine the fractional part */
	fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

	/* Implement the fractional part in the register */
	if ((USART2->CR1 & USART_CR1_OVER8) != 0) {
		tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t) 0x07);
	} else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
	{
		tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t) 0x0F);
	}

	/* Write to USART BRR register */
	USART2->BRR = (uint16_t) tmpreg;

}

STM32F411USART2* STM32F411USART2::instance = 0;

STM32F411USART2* STM32F411USART2::getInstance() {
	if (instance == 0)
		instance = new STM32F411USART2();
	return instance;
}

uint8_t STM32F411USART2::read() {
	return rxBuffer.rem();
}

void STM32F411USART2::write(uint8_t data) {
	txBuffer.add(data);
	if (!isTransmitting) {
		isTransmitting = true;
		USART2->CR1 |= USART_CR1_TXEIE; // active l'interruption.
	}

}

void STM32F411USART2::sendByteToString(uint8_t byte) {
	char buffer[33];
	itoa((int) byte, buffer, 10);
	sendString(buffer);
}

void STM32F411USART2::sendbyteToString(uint16_t byte) {
	char buffer[33];
	itoa(byte, buffer, 10);
	sendString(buffer);
}

void STM32F411USART2::sendBytes(uint8_t* data, uint32_t nBytes) {
	for (uint32_t i = 0; i < nBytes; i++) {
		write(*data++);
	}
}
void STM32F411USART2::sendByte8ToBinaryString(uint8_t data) {
	for (int i = 0; i < 8; i++) {
		((data >> (7 - i)) & 0x01) ?
				STM32F411USART2::getInstance()->write('1') :
				STM32F411USART2::getInstance()->write('0');
	}
	STM32F411USART2::getInstance()->write('\n\r');
}

void STM32F411USART2::sendByte16ToBinaryString(uint16_t data) {
	for (int i = 0; i < 16; i++) {
		if ((data >> (15 - i)) & 0x01)
			STM32F411USART2::getInstance()->write('1');
		else
			STM32F411USART2::getInstance()->write('0');
	}
	STM32F411USART2::getInstance()->write('\n\r');
}

void STM32F411USART2::sendByte32ToBinaryString(uint32_t data) {
	for (int i = 0; i < 32; i++) {
		if ((data >> (31 - i)) & 0x01)
			STM32F411USART2::getInstance()->write('1');
		else
			STM32F411USART2::getInstance()->write('0');
	}
	STM32F411USART2::getInstance()->write('\n\r');
}

void STM32F411USART2::sendString(const char *s) {
	while (*s) {
		write(*s++); // Send Char
	}
}

void STM32F411USART2::sendString(uint8_t *u) {
	while (*u) {
		write(*u++); // Send Char
	}
}

bool STM32F411USART2::dataAvailable() const {
	return !rxBuffer.isEmpty();
}

void USART2_IRQHandler(void) {
	volatile unsigned int isr;

	isr = USART2->SR;

	// RX Data
	if (isr & USART_SR_RXNE) {
		USART2->SR &= ~USART_SR_RXNE;
		STM32F411USART2::instance->rxBuffer.add(USART2->DR);
	}
	// TX Done
	if ((isr & USART_SR_TXE)) {
		USART2->SR &= ~USART_SR_TXE;
		if (STM32F411USART2::instance->txBuffer.isEmpty()) {
			STM32F411USART2::instance->isTransmitting = false;
			USART2->CR1 &= (~USART_CR1_TXEIE);
		} else {
			USART2->DR = STM32F411USART2::instance->txBuffer.rem();
			STM32F411USART2::instance->isTransmitting = true;
		}
	}
}

