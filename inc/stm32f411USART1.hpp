/*
 * com.hpp
 *
 *  Created on: Mar 27, 2019
 *      Author: rev
 */

#ifndef STM32F411USART1_HPP_
#define STM32F411USART1_HPP_

#include "stm32f4xx_usart.h"
#include "hardware.h"
#include "buffer.hpp"
#include "flash.hpp"
#include "stm32f4timer.hpp"


extern "C" void TIM3_IRQHandler(void);
extern "C" void USART1_IRQHandler(void);


class STM32F411USART1 {
public:

	static STM32F411USART1* getInstance();
	virtual ~STM32F411USART1();
	void write(uint8_t data);
	void sendBytes(uint8_t *data, uint32_t nBytes);
	void sendByteToString(uint8_t byte);
	void sendbyteToString(uint16_t byte);
	void sendByte8ToBinaryString(uint8_t data);
	void sendByte16ToBinaryString(uint16_t data);
	void sendByte32ToBinaryString(uint32_t data);
	void sendString(const char *s);
	void sendString(uint8_t *u);
	uint8_t read();
	void incommingDataDecoder(Flash* flash);
	bool dataAvailable();
	void setBaudRate(uint32_t baudrate);
	void setEcho(bool state);
	void parseTram(Flash *flash);
	void sendFilenameList(Flash *flash);

	bool readyTotransfer;
	bool okToTransfer;
private:

	STM32F411USART1();
	bool echo;
	bool isTransmitting;
	Buffer<uint8_t, 128> rxBuffer;
	Buffer<uint8_t, 128> txBuffer;
	static STM32F411USART1* instance;



	friend void USART1_IRQHandler(void);
	friend void TIM3_IRQHandler(void);

	enum parseTram_e {
		WAIT, RXPAYLOAD, VALIDATE
	} parseRxTram = WAIT;

	enum commState_e {
		IDLE, WAIT_OK_TO_TRANSFER, ASK_FILE_TO_SERVER, SAVE_FILE, SAVE_FILENAME, TRANSFER_COMPLETED, TRANSFER_FAILED
	} commState = IDLE;

	uint16_t commRxCnt = 0;
	uint8_t cmdCntr = 0;
	static constexpr uint8_t TRAM_SIZE = 6;
	char tram[TRAM_SIZE + 2];

	uint16_t rxColumnNo = 0;
	uint16_t rxImageNo = 0;
	uint8_t pixelColumnBuffer[1156];
	uint16_t pixelColumnBufferCntr;
	char filename[256];

	bool timeout = 0;
	uint8_t timeoutCntr = 0;
	uint8_t retry = 0;
	static constexpr uint8_t TIME_OUT = 2;
	static constexpr uint8_t MAX_RETRY = 3;

	const char CMD_ImageReadyToTransfer[7] = { "imgrdy" };
	const char CMD_GetFilenameList[7] = { "getlst" };
	const char CMD_Filename[7] = { "lfname" };
	const char CMD_Red[4] = { "red" };
	const char CMD_Green[4] = { "grn" };
	const char CMD_Blue[4] = { "blu" };
	const char CMD_GlobalBrightness[4] = { "glb" };
	const char CMD_Del0[7] = { "del000" };
	const char CMD_Del1[7] = { "del001" };
	const char CMD_Del2[7] = { "del002" };
	const char CMD_Del3[7] = { "del003" };
	const char CMD_Del4[7] = { "del004" };
	const char CMD_Del5[7] = { "del005" };
	const char CMD_Del6[7] = { "del006" };
	const char CMD_Del7[7] = { "del007" };
	const char CMD_Del8[7] = { "del008" };
	const char CMD_Del9[7] = { "del009" };
	const char CMD_Del10[7] = { "del010" };
	const char CMD_Del11[7] = { "del011" };
	const char CMD_Del12[7] = { "del012" };
	const char CMD_Del13[7] = { "del013" };
	const char CMD_Del14[7] = { "del014" };

	//STM32F4Timer *commTimeOut;

};

#endif /* STM32F411USART1_HPP_ */
