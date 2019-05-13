/*
 * stm32spi2.hpp
 *
 *  Created on: Feb 28, 2019
 *      Author: Zandwich
 */

#ifndef STM32SPI2_HPP_
#define STM32SPI2_HPP_

#include "stm32f4xx_spi.h"
#include "hardware.h"
#include "ispi.hpp"

class STM32SPI2 : public ISPI {
public:

	STM32SPI2();
	~STM32SPI2();
	void init();
	void setBitBang();
	void sendControlBits();
	void sendManualByte(uint8_t data);
	void sendByte16(uint16_t data);
	void sendByte8(uint8_t data);
	uint16_t receiveData();
	void assert();
	void deassert();

private:
	static constexpr uint8_t ControlDataByte	= 0x96;
};



#endif /* STM32SPI2_HPP_ */
