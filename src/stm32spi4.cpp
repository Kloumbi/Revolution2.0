/*
 * stm32spi4.cpp
 *
 *  Created on: Mar 1, 2019
 *      Author: Zandwich
 */

#include "stm32spi4.hpp"

STM32SPI4::STM32SPI4()
{

}

STM32SPI4::~STM32SPI4()
{

}

void STM32SPI4::init()
{
	//------------------------GPIOA------------------------------------------
		GPIO_InitTypeDef GPIOA_InitStructure;
		GPIOA_InitStructure.GPIO_Pin = SPI4_MOSI_Pin;
		GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIOA_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIOA_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_Init(SPI4_MOSI_GPIO, &GPIOA_InitStructure);

		GPIO_PinAFConfig(SPI4_MOSI_GPIO,SPI4_MOSI_PinSource,SPI4_MOSI_ALTERNATE_FUNCTION);

		//------------------------GPIOB------------------------------------------
		GPIO_InitTypeDef GPIOB_InitStructure;
		GPIOB_InitStructure.GPIO_Pin = SPI4_CLK_Pin;
		GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIOB_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIOB_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_Init(SPI4_CLK_GPIO, &GPIOB_InitStructure);

		GPIO_PinAFConfig(SPI4_CLK_GPIO,SPI4_CLK_PinSource,SPI4_CLK_ALTERNATE_FUNCTION);

		SPI_InitTypeDef SPI_InitStruct;
		SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;
		SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
		SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStruct.SPI_CRCPolynomial = 0;
		SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
		SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB ;
		SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

		SPI_Init(SPI4,&SPI_InitStruct);

		SPI_Cmd(SPI4,ENABLE);
}

void STM32SPI4::setBitBang()
{
	//------------------------GPIOA------------------------------------------
		GPIO_InitTypeDef GPIOA_InitStructure;
		GPIOA_InitStructure.GPIO_Pin = SPI4_MOSI_Pin;
		GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIOA_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIOA_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_Init(SPI4_MOSI_GPIO, &GPIOA_InitStructure);

		//------------------------GPIOB------------------------------------------
		GPIO_InitTypeDef GPIOB_InitStructure;
		GPIOB_InitStructure.GPIO_Pin = SPI4_CLK_Pin;
		GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIOB_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIOB_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_Init(SPI4_CLK_GPIO, &GPIOB_InitStructure);
}

void STM32SPI4::sendByte8(uint8_t data)
{
	SPI_SendData(SPI4,data);
	while(SPI_GetFlagStatus(SPI4,SPI_FLAG_TXE)==RESET);
}

void STM32SPI4::sendByte16(uint16_t data)
{
	SPI_I2S_SendData(SPI4,data);

	while(SPI_I2S_GetFlagStatus(SPI4,SPI_FLAG_TXE)==RESET);
}

void STM32SPI4::sendManualByte(uint8_t data)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// consider leftmost bit
		// set line high if bit is 1, low if bit is 0
		if (data & 0x80)
			GPIO_SetBits(SPI4_MOSI_GPIO,SPI4_MOSI_Pin);
		else
			GPIO_ResetBits(SPI4_MOSI_GPIO,SPI4_MOSI_Pin);

		// pulse clock to indicate that bit value should be read
		GPIO_ResetBits(SPI4_CLK_GPIO,SPI4_CLK_Pin);

		GPIO_SetBits(SPI4_CLK_GPIO,SPI4_CLK_Pin);

		// shift byte left so next bit will be leftmost
		data <<= 1;
	}
}

void STM32SPI4::sendControlBits()
{
	uint8_t data = ControlDataByte;
	GPIO_SetBits(SPI4_MOSI_GPIO,SPI4_MOSI_Pin);

	GPIO_ResetBits(SPI4_CLK_GPIO,SPI4_CLK_Pin);

	GPIO_SetBits(SPI4_CLK_GPIO,SPI4_CLK_Pin);

	GPIO_ResetBits(SPI4_CLK_GPIO,SPI4_CLK_Pin);

	for (uint8_t i = 0; i < 8; i++)
	{
		// consider leftmost bit
		// set line high if bit is 1, low if bit is 0
		if (data & 0x80)
			GPIO_SetBits(SPI4_MOSI_GPIO,SPI4_MOSI_Pin);
		else
			GPIO_ResetBits(SPI4_MOSI_GPIO,SPI4_MOSI_Pin);

		// pulse clock to indicate that bit value should be read
		GPIO_ResetBits(SPI4_CLK_GPIO,SPI4_CLK_Pin);

		GPIO_SetBits(SPI4_CLK_GPIO,SPI4_CLK_Pin);

		// shift byte left so next bit will be leftmost
		data <<= 1;
	}
}

uint16_t STM32SPI4::receiveData()
{
	assert();
	uint16_t temp;
	temp = SPI_ReceiveData(SPI4);
	deassert();
	return temp;
}

void STM32SPI4::assert()
{
	//SPI_Cmd(SPI4,ENABLE);
	SPI_NSSInternalSoftwareConfig(SPI4,SPI_NSSInternalSoft_Set);
}
void STM32SPI4::deassert()
{
	SPI_NSSInternalSoftwareConfig(SPI4,SPI_NSSInternalSoft_Reset);
	//SPI_Cmd(SPI4,DISABLE);
}


