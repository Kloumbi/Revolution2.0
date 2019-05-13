/*
 * STM32F411USART2.hpp
 *
 *  Created on: Apr 24, 2019
 *      Author: rev
 */

#ifndef STM32F411USART2_HPP_
#define STM32F411USART2_HPP_

#include "stm32f4xx.h"
#include "BufferTemplate.h"

extern "C" void USART2_IRQHandler(void);


class STM32F411USART2 {
	friend void USART2_IRQHandler(void);
	bool isTransmitting = false;
	BufferTemplate<uint8_t,64> rxBuffer;
	BufferTemplate<uint8_t,64> txBuffer;
	STM32F411USART2();
	static STM32F411USART2 * instance;
public:
	static STM32F411USART2 * getInstance();
	virtual ~STM32F411USART2();
	void setBaudRate(uint32_t baudrate);
	void write(uint8_t data);
	void sendByteToString(uint8_t byte);
	void sendbyteToString(uint16_t byte);
	void sendBytes(uint8_t *data, uint32_t nBytes);
	void sendByte8ToBinaryString(uint8_t data);
	void sendByte16ToBinaryString(uint16_t data);
	void sendByte32ToBinaryString(uint32_t data);
	void sendString(const char *s);
	void sendString(uint8_t *u);
	bool dataAvailable() const;
	uint8_t read();
};



#endif /* STM32F411USART2_HPP_ */
