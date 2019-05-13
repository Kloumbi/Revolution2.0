/*
 * stm32spi1.cpp
 *
 *  Created on: Feb 28, 2019
 *      Author: rev
 */

#include "stm32spi1.hpp"
#include <string.h>
STM32SPI1::STM32SPI1()
{

}

STM32SPI1::~STM32SPI1()
{

}

void STM32SPI1::init()
{

	//------------------------GPIOA init------------------------------------------
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SPI1_MOSI_Pin | SPI1_CLK_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(SPI1_CLK_GPIO,SPI1_CLK_PinSource,SPI1_ALTERNATE_FUNCTION);
	GPIO_PinAFConfig(SPI1_MOSI_GPIO,SPI1_MOSI_PinSource,SPI1_ALTERNATE_FUNCTION);


	//----------------------SPI init------------------------------------------------
	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx ;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CRCPolynomial = 0;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB ;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI1,&SPI_InitStruct);

	SPI_Cmd(SPI1,ENABLE);
}

void STM32SPI1::sendManualByte(uint8_t data)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// consider leftmost bit
		// set line high if bit is 1, low if bit is 0
		if (data & 0x80)
			GPIO_SetBits(SPI1_MOSI_GPIO,SPI1_MOSI_Pin);
		else
			GPIO_ResetBits(SPI1_MOSI_GPIO,SPI1_MOSI_Pin);

		// pulse clock to indicate that bit value should be read
		GPIO_ResetBits(SPI1_CLK_GPIO,SPI1_CLK_Pin);

		GPIO_SetBits(SPI1_CLK_GPIO,SPI1_CLK_Pin);

		// shift byte left so next bit will be leftmost
		data <<= 1;
	}
}

void STM32SPI1::sendByte8(uint8_t data)
{
	//while(SPI_I2S_GetFlagStatus(SPI1,SPI_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,data);

	while(SPI_I2S_GetFlagStatus(SPI1,SPI_FLAG_TXE)==RESET);

}

void STM32SPI1::sendByte16(uint16_t data)
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_FLAG_TXE)==RESET);
	SPI1->DR = data;
}

void STM32SPI1::sendControlBits()
{

	uint8_t data = ControlDataByte;
	GPIO_SetBits(SPI1_MOSI_GPIO,SPI1_MOSI_Pin);

	GPIO_ResetBits(SPI1_CLK_GPIO,SPI1_CLK_Pin);

	GPIO_SetBits(SPI1_CLK_GPIO,SPI1_CLK_Pin);

	GPIO_ResetBits(SPI1_CLK_GPIO,SPI1_CLK_Pin);

	for (uint8_t i = 0; i < 8; i++)
	{
		// consider leftmost bit
		// set line high if bit is 1, low if bit is 0
		if (data & 0x80)
			GPIO_SetBits(SPI1_MOSI_GPIO,SPI1_MOSI_Pin);
		else
			GPIO_ResetBits(SPI1_MOSI_GPIO,SPI1_MOSI_Pin);

		// pulse clock to indicate that bit value should be read
		GPIO_ResetBits(SPI1_CLK_GPIO,SPI1_CLK_Pin);

		GPIO_SetBits(SPI1_CLK_GPIO,SPI1_CLK_Pin);

		// shift byte left so next bit will be leftmost
		data <<= 1;
	}

}

uint16_t STM32SPI1::receiveData()
{
	assert();
	uint16_t temp;
	temp = SPI_I2S_ReceiveData(SPI1);
	deassert();
	return temp;
}

void STM32SPI1::assert()
{
	//SPI_Cmd(SPI1,ENABLE);
	SPI_NSSInternalSoftwareConfig(SPI1,SPI_NSSInternalSoft_Set);
}
void STM32SPI1::deassert()
{
	SPI_NSSInternalSoftwareConfig(SPI1,SPI_NSSInternalSoft_Reset);
	//SPI_Cmd(SPI1,DISABLE);
}

void STM32SPI1::setBitBang()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SPI1_MOSI_Pin | SPI1_CLK_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
