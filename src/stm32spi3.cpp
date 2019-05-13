/*
 * stm32spi3.cpp
 *
 *  Created on: Mar 1, 2019
 *      Author: Zandwich
 */

#include "stm32spi3.hpp"

STM32SPI3::STM32SPI3()
{

}

STM32SPI3::~STM32SPI3()
{

}

void STM32SPI3::init()
{
	//------------------------GPIOB------------------------------------------
		GPIO_InitTypeDef GPIOB_InitStructure;
		GPIOB_InitStructure.GPIO_Pin = SPI3_CLK_Pin;
		GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIOB_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIOB_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_Init(SPI3_CLK_GPIO, &GPIOB_InitStructure);

		GPIO_PinAFConfig(SPI3_CLK_GPIO,SPI3_CLK_PinSource,SPI3_CLK_ALTERNATE_FUNCTION);

		//------------------------GPIOC------------------------------------------
		GPIO_InitTypeDef GPIOC_InitStructure;
		GPIOC_InitStructure.GPIO_Pin = SPI3_MOSI_Pin;
		GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIOC_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIOC_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_Init(SPI3_MOSI_GPIO, &GPIOC_InitStructure);

		GPIO_PinAFConfig(SPI3_MOSI_GPIO,SPI3_MOSI_PinSource,SPI3_MOSI_ALTERNATE_FUNCTION);

		SPI_InitTypeDef SPI_InitStruct;
		SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;
		SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
		SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStruct.SPI_CRCPolynomial = 0;
		SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
		SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB ;
		SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

		SPI_Init(SPI3,&SPI_InitStruct);
		SPI_Cmd(SPI3,ENABLE);
}

void STM32SPI3::setBitBang()
{
	//------------------------GPIOB------------------------------------------
		GPIO_InitTypeDef GPIOB_InitStructure;
		GPIOB_InitStructure.GPIO_Pin = SPI3_CLK_Pin;
		GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIOB_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIOB_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_Init(SPI3_CLK_GPIO, &GPIOB_InitStructure);

		//------------------------GPIOC------------------------------------------
		GPIO_InitTypeDef GPIOC_InitStructure;
		GPIOC_InitStructure.GPIO_Pin = SPI3_MOSI_Pin;
		GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIOC_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIOC_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_Init(SPI3_MOSI_GPIO, &GPIOC_InitStructure);
}

void STM32SPI3::sendByte8(uint8_t data)
{
	SPI_SendData(SPI3,data);
	while(SPI_GetFlagStatus(SPI3,SPI_FLAG_TXE)==RESET);
}

void STM32SPI3::sendByte16(uint16_t data)
{
	SPI_I2S_SendData(SPI3,data);

	while(SPI_I2S_GetFlagStatus(SPI3,SPI_FLAG_TXE)==RESET);
}

void STM32SPI3::sendManualByte(uint8_t data)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// consider leftmost bit
		// set line high if bit is 1, low if bit is 0
		if (data & 0x80)
			GPIO_SetBits(SPI3_MOSI_GPIO,SPI3_MOSI_Pin);
		else
			GPIO_ResetBits(SPI3_MOSI_GPIO,SPI3_MOSI_Pin);

		// pulse clock to indicate that bit value should be read
		GPIO_ResetBits(SPI3_CLK_GPIO,SPI3_CLK_Pin);

		GPIO_SetBits(SPI3_CLK_GPIO,SPI3_CLK_Pin);

		// shift byte left so next bit will be leftmost
		data <<= 1;
	}
}

void STM32SPI3::sendControlBits()
{
	uint8_t data = ControlDataByte;
	GPIO_SetBits(SPI3_MOSI_GPIO,SPI3_MOSI_Pin);

	GPIO_ResetBits(SPI3_CLK_GPIO,SPI3_CLK_Pin);

	GPIO_SetBits(SPI3_CLK_GPIO,SPI3_CLK_Pin);

	GPIO_ResetBits(SPI3_CLK_GPIO,SPI3_CLK_Pin);

	for (uint8_t i = 0; i < 8; i++)
	{
		// consider leftmost bit
		// set line high if bit is 1, low if bit is 0
		if (data & 0x80)
			GPIO_SetBits(SPI3_MOSI_GPIO,SPI3_MOSI_Pin);
		else
			GPIO_ResetBits(SPI3_MOSI_GPIO,SPI3_MOSI_Pin);

		// pulse clock to indicate that bit value should be read
		GPIO_ResetBits(SPI3_CLK_GPIO,SPI3_CLK_Pin);

		GPIO_SetBits(SPI3_CLK_GPIO,SPI3_CLK_Pin);

		// shift byte left so next bit will be leftmost
		data <<= 1;
	}
}

uint16_t STM32SPI3::receiveData()
{
	assert();
	uint16_t temp;
	temp = SPI_I2S_ReceiveData(SPI3);
	deassert();
	return temp;
}

void STM32SPI3::assert()
{
	//SPI_Cmd(SPI3,ENABLE);
	SPI_NSSInternalSoftwareConfig(SPI3,SPI_NSSInternalSoft_Set);
}
void STM32SPI3::deassert()
{
	SPI_NSSInternalSoftwareConfig(SPI3,SPI_NSSInternalSoft_Reset);
	//SPI_Cmd(SPI3,DISABLE);
}


