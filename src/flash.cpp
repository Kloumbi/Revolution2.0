/*
 * flash.cpp
 *
 *  Created on: Mar 1, 2019
 *      Author: Kloumbi
 */
#include <flash.hpp>
#include <string>
#include <string.h>
#include <math.h>
#include <stm32f411USART1.hpp>
#include "stm32f411USART2.hpp"

Flash* Flash::instance = 0;
STM32F411USART1 *terminal = STM32F411USART1::getInstance();

Flash::Flash(bool debugEnable) {
	debug = debugEnable;
}

Flash::~Flash() {
	savePositionOfPresentImagesInCarrousel();
	if (instance)
		delete instance;
}

Flash* Flash::getInstance(bool debug) {
	if (instance == 0)
		instance = new Flash(debug);
	return instance;
}

void Flash::init() {

	if (debug)
		terminal->sendString("\n\rInit start\n\r\r");

	//------------------------ GPIO_B CS SCLK ------------------------------------------
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SPI5_CLK_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(SPI5_CLK_GPIO, &GPIO_InitStructure);
	GPIO_PinAFConfig(SPI5_CLK_GPIO, SPI5_CLK_PinSource,
			SPI5_ALTERNATE_FUNCTION);

	GPIO_InitStructure.GPIO_Pin = SPI5_NSS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(SPI5_NSS_GPIO, &GPIO_InitStructure);

	//------------------------ GPIO_A MOSI MISO------------------------------------------

	GPIO_InitStructure.GPIO_Pin = SPI5_MOSI_Pin | SPI5_MISO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(SPI5_MOSI_GPIO, SPI5_MOSI_PinSource,
			SPI5_ALTERNATE_FUNCTION);
	GPIO_PinAFConfig(SPI5_MISO_GPIO, SPI5_MISO_PinSource,
			SPI5_ALTERNATE_FUNCTION);

	GPIO_SetBits(SPI5_NSS_GPIO, SPI5_NSS_Pin);

	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI5, &SPI_InitStruct);
	SPI_NSSInternalSoftwareConfig(SPI5, SPI_NSSInternalSoft_Set);
	SPI_Cmd(SPI5, ENABLE);

	while (isBusy())
		;

	positionOfPresentImages = getPositionOfPresentImagesInCarrousel();
	readControlRegister();

	if (debug)
		terminal->sendString("\n\rFlash initialization completed.\n\r");
}

void Flash::setCS(bool state) {
	if (state) {
		GPIO_ResetBits(SPI5_NSS_GPIO, SPI5_NSS_Pin);
	} else {
		GPIO_SetBits(SPI5_NSS_GPIO, SPI5_NSS_Pin);
	}
}

uint8_t Flash::spiTransfer(uint8_t data) {
	uint8_t result = 0;
	while (SPI_GetFlagStatus(SPI5, SPI_FLAG_TXE) == RESET)
		;
	SPI5->DR = data;
	while (SPI_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET)
		;
	result = SPI5->DR;
	return result;
}

void Flash::getDeviceID(uint8_t *buffer) {
	setCS(true);
	spiTransfer(DeviceID);
	for (int i = 0; i < 5; i++) {
		buffer[i] = spiTransfer(DummyByte);
		if (debug) {

			terminal->sendByte8ToBinaryString(buffer[i]);
			terminal->sendString("\n\r");

		}
	}
	setCS(false);
}

uint16_t Flash::readStatusRegister() {
	uint16_t a, b;
	setCS(true);
	spiTransfer(StatusRegisterRead);
	a = spiTransfer(DummyByte);
	b = spiTransfer(DummyByte);
	setCS(false);
	return a << 8 | b;
}

void Flash::readStatusRegisterToString() {
	uint16_t a, b;
	setCS(true);
	spiTransfer(StatusRegisterRead);
	a = spiTransfer(DummyByte);
	b = spiTransfer(DummyByte);
	setCS(false);
	if (debug) {
		terminal->sendString("Reading status register :\n\r");

		(a & 0b10000000) ?
				terminal->sendString("b7 : RDY/BUSY Ready\n\r") :
				terminal->sendString("b7 : RDY/BUSY Busy\n\r");
		(a & 0b01000000) ?
				terminal->sendString("b6 : COMP Flase\n\r") :
				terminal->sendString("b6 : COMP true\n\r");
		(a & 0b00110100) ?
				terminal->sendString("b5:2 : DENSITY 32-Mbit\n\r") :
				terminal->sendString("b5:2 : DENSITY ???\n\r");
		(a & 0b00000010) ?
				terminal->sendString("b1 : PROTECT enable\n\r") :
				terminal->sendString("b1 : PROTECT disable\n\r");
		(a & 0b00000001) ?
				terminal->sendString("b0 : PAGE SIZE 512B\n\r") :
				terminal->sendString("b0 : PAGE SIZE 528B\n\r");
		(b & 0b10000000) ?
				terminal->sendString("b7 : RDY/BUSY Ready\n\r") :
				terminal->sendString("b7 : RDY/BUSY Busy\n\r");
		(b & 0b01000000) ?
				terminal->sendString("b6 : RES\n\r") :
				terminal->sendString("b6 : RES\n\r");
		(b & 0b00100000) ?
				terminal->sendString("b5 : EPE Detected\n\r") :
				terminal->sendString("b5 : EPE Ok\n\r");
		(b & 0b00100000) ?
				terminal->sendString("b4 : RES\n\r") :
				terminal->sendString("b4 : RES\n\r");
		(b & 0b00100000) ?
				terminal->sendString("b3 : SLE Enable\n\r") :
				terminal->sendString("b3 : SLE Disable\n\r");
		(b & 0b00100000) ?
				terminal->sendString("b2 : PS2 Is\n\r") :
				terminal->sendString("b2 : PS2 Not\n\r");
		(b & 0b00000010) ?
				terminal->sendString("b1 : PS1 Is\n\r") :
				terminal->sendString("b1 : PS2 Not\n\r");
		(b & 0b00000001) ?
				terminal->sendString("b0 : ES Is\n\r") :
				terminal->sendString("b0 : ES Not\n\r");

	}
}

uint8_t Flash::readConfigurationRegister() {
	uint8_t t = 0;
	setCS(true);
	spiTransfer(ConfigurationRegisterRead);
	t = spiTransfer(DummyByte);
	setCS(false);
	return t;
}

void Flash::setPageSizeBinary() {
	if (debug)

		terminal->sendString(
				"Configuring page size of 512 bytes (power of 2 adresses)\n\r");

	setCS(true);
	for (int i = 0; i < 4; i++) {
		spiTransfer(BinaryPageSize[i]);
	}
	setCS(false);
	while (isBusy())
		;
	if (debug)

		terminal->sendString("Configuration completed\n\r");

}

bool Flash::isBusy() {
	return !(readStatusRegister() & BusyFlag);
}

uint8_t Flash::readByte(const address_t *add) {
	uint8_t incommingByte;
	uint32_t address = 0;
	address = add->page;
	address = address << 9;
	address |= add->byte;

	setCS(true);
	spiTransfer(MainMemmoryPageRead);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));
	spiTransfer(DummyByte);
	spiTransfer(DummyByte);
	spiTransfer(DummyByte);
	spiTransfer(DummyByte);
	incommingByte = spiTransfer(DummyByte);
	setCS(false);
	return incommingByte;
}

void Flash::readByte(const address_t *add, uint8_t *buffer, uint16_t nByte) {
	uint32_t address = 0;
	address = add->page;
	address = address << 9;
	address |= add->byte;

	setCS(true);
	spiTransfer(MainMemmoryPageRead);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));
	spiTransfer(DummyByte);
	spiTransfer(DummyByte);
	spiTransfer(DummyByte);
	spiTransfer(DummyByte);

	for (uint16_t i = 0; i < nByte; i++) {
		buffer[i] = spiTransfer(DummyByte);
	}
	setCS(false);
}
void Flash::readByte(const address_t *add, char *buffer, uint16_t nByte) {
	uint32_t address = 0;
	address = add->page;
	address = address << 9;
	address |= add->byte;

	setCS(true);
	spiTransfer(MainMemmoryPageRead);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));
	spiTransfer(DummyByte);
	spiTransfer(DummyByte);
	spiTransfer(DummyByte);
	spiTransfer(DummyByte);

	for (uint16_t i = 0; i < nByte; i++) {
		buffer[i] = spiTransfer(DummyByte);
	}
	setCS(false);
}
void Flash::readPageArray(const address_t *add, uint8_t* buffer,
		uint32_t nBytes) {
	uint32_t address = 0;
	address = add->page;
	address = address << 9;
	address |= add->byte;

	setCS(true);
	spiTransfer(ContinuousPageRead);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));

	spiTransfer(DummyByte);
	spiTransfer(DummyByte);

	for (uint32_t i = 0; i < nBytes; i++) {
		buffer[i] = spiTransfer(DummyByte);
	}
	setCS(false);
}

void Flash::writeByte(const address_t *add, uint8_t byte) {
	uint32_t address = 0;
	address = add->page;
	address = address << 9;
	address |= add->byte;

	setCS(true);
	spiTransfer(WrtitePagesThroughBuf1BIE);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));
	spiTransfer(byte);
	setCS(false);

	while (!(readStatusRegister() & BusyFlag))
		;
}
void Flash::writeByte(const address_t *add, int byte) {
	uint32_t address = 0;
	address = add->page;
	address = address << 9;
	address |= add->byte;

	setCS(true);
	spiTransfer(WrtitePagesThroughBuf1BIE);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));
	spiTransfer(byte);
	setCS(false);

	while (!(readStatusRegister() & BusyFlag))
		;
}
void Flash::writeByte(const address_t *add, uint16_t byte) {
	uint32_t address = 0;
	address = add->page;
	address = address << 9;
	address |= add->byte;

	setCS(true);
	spiTransfer(WrtitePagesThroughBuf1BIE);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));
	for (int i = 0; i < 2; i++) {
		spiTransfer(byte >> (16 - (8 * i)));
	}
	setCS(false);

	while (!(readStatusRegister() & BusyFlag))
		;
}
void Flash::writeByte(const address_t* add, uint32_t byte) {
	uint32_t address = 0;
	address = add->page;
	address = address << 9;
	address |= add->byte;

	setCS(true);
	spiTransfer(WrtitePagesThroughBuf1BIE);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));
	for (int i = 0; i < 4; i++) {
		spiTransfer(byte >> (24 - (8 * i)));
	}
	setCS(false);

	while (!(readStatusRegister() & BusyFlag))
		;
}

void Flash::writeByte(const address_t *add, uint8_t *byte, uint16_t nByte,
		uint16_t offsetByte) {
	uint32_t address = 0;
	address = add->page;
	address = address << 9;
	address |= add->byte;

	setCS(true);
	spiTransfer(WrtitePagesThroughBuf1BIE);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));
	for (uint16_t i = 0; i < nByte; i++) {
		spiTransfer(byte[i + offsetByte]);
	}
	setCS(false);

	while (!(readStatusRegister() & BusyFlag))
		;
}

void Flash::writeByte(const address_t *add, const char *byte, uint16_t nByte) {
	uint8_t data;

	if (debug)
		terminal->sendString("Writing byte\n\r\r");

	uint32_t address = 0;
	address = add->page;
	address = address << 9;
	address |= add->byte;

	setCS(true);
	spiTransfer(WrtitePagesThroughBuf2BIE);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));
	for (uint16_t i = 0; i < nByte; i++) {
		data = (uint8_t) byte[i];
		spiTransfer(data);
	}
	setCS(false);
	while (!(readStatusRegister() & BusyFlag))
		;
	if (debug)
		terminal->sendString("Done. \n\r\r");
}

void Flash::erasePage(const address_t *add) {
	uint32_t address = 0;
	address = add->page;
	address = address << 9;

	if (debug)

		terminal->sendString("Erasing page...\n\r");

	setCS(true);
	spiTransfer(MainMemmoryPageRead);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));
	setCS(false);
	while (!(readStatusRegister() & BusyFlag))
		;

	if (debug)
		terminal->sendString("Erase successful...\n\r");

}

void Flash::eraseChip() {
	if (debug)

		terminal->sendString("Erasing Chip...\n\r");

	setCS(true);
	spiTransfer(ChipErase[0]);
	spiTransfer(ChipErase[1]);
	spiTransfer(ChipErase[2]);
	spiTransfer(ChipErase[3]);
	setCS(false);
	while (!(readStatusRegister() & BusyFlag))
		;
	if (debug)

		terminal->sendString("Erase successful...\n\r");

}
void Flash::disableSectorProtection() {
	if (debug)
		terminal->sendString("Disabling sector protection...\n\r");

	setCS(true);
	spiTransfer(DisableSectorProtect[0]);
	spiTransfer(DisableSectorProtect[1]);
	spiTransfer(DisableSectorProtect[2]);
	spiTransfer(DisableSectorProtect[3]);
	setCS(false);
	while (!(readStatusRegister() & BusyFlag))
		;
	if (debug)
		terminal->sendString("Done.\n\r");
}
void Flash::readControlRegister() {
	redMaxCurrent = readByte(&RedMaxCurrentSettingAddress);
	greenMaxCurrent = readByte(&GreenMaxCurrentSettingAddress);
	blueMaxCurrent = readByte(&BlueMaxCurrentSettingAddress);
	globalBrightness = readByte(&GlobalBrightnessSettingAddress);
}

void Flash::writeControlRegister() {
	writeByte(&RedMaxCurrentSettingAddress, redMaxCurrent);
	writeByte(&GreenMaxCurrentSettingAddress, greenMaxCurrent);
	writeByte(&BlueMaxCurrentSettingAddress, blueMaxCurrent);
	writeByte(&GlobalBrightnessSettingAddress, globalBrightness);
}

uint32_t Flash::getPositionOfPresentImagesInCarrousel() {
	uint8_t bytes[4];
	uint32_t result;
	readPageArray(&PositionOfPresentImagesInCarrouselAddress, bytes, 4);
	result = bytes[0];
	result = (result << 8) | bytes[1];
	result = (result << 8) | bytes[2];
	result = (result << 8) | bytes[3];
	return result;
}

void Flash::setImageInCarrousel(uint8_t imageNo) {
	imageNo = imageNo % MaxImageStored;
	positionOfPresentImages |= 1 << (imageNo);
	savePositionOfPresentImagesInCarrousel();
}

void Flash::savePositionOfPresentImagesInCarrousel() {
	writeByte(&PositionOfPresentImagesInCarrouselAddress,
			positionOfPresentImages);
}

uint8_t Flash::getNumberOfImagesInCarrousel() {
	return countSetBits(positionOfPresentImages);
}

bool Flash::savePixelColumn(uint8_t imageNo, uint8_t columnNo,
		uint8_t* source) {
	if (debug)

		terminal->sendString("Saving pixel column to flash...\n\r");

	imageNo = imageNo % MaxImageStored;

	uint32_t imageColumnStartPage = FirstImagePageAddress
			+ (imageNo * PagesPerImage);
	uint32_t pixelColumnStartPage = imageColumnStartPage
			+ (floor((columnNo * ColumnPixelArraySize) / PageSize));
	uint32_t pixelColumnStartByte = (columnNo * ColumnPixelArraySize)
			% PageSize;

	if (debug) {
		terminal->sendString("\n\rpixelColumnPageOffset :");
		terminal->sendByte32ToBinaryString(pixelColumnStartPage);
		terminal->sendString("\n\rpixelColumnStartPage :");
		terminal->sendByte32ToBinaryString(imageColumnStartPage);
		terminal->sendString("\n\rpixelColumnStartByte :");
		terminal->sendByte32ToBinaryString(pixelColumnStartByte);

	}

	address_t add;
	add.page = pixelColumnStartPage;
	add.byte = pixelColumnStartByte;

	uint16_t payloadSize = ColumnPixelArraySize;

	writeByte(&add, source, PageSize - pixelColumnStartByte);
	payloadSize -= (PageSize - pixelColumnStartByte);
	uint8_t pageToWrite = floor(payloadSize / PageSize);
	for (uint8_t i = 0; i < pageToWrite; i++) {
		add.page++;
		add.byte = 0;
		writeByte(&add, source, PageSize, ColumnPixelArraySize - payloadSize);
		payloadSize -= PageSize;
	}

	add.page++;
	add.byte = 0;
	writeByte(&add, source, payloadSize, ColumnPixelArraySize - payloadSize);
	payloadSize -= payloadSize;

	if (debug)
		terminal->sendString("Pixel column saved to flash...\n\r");

	return !payloadSize;
}

bool Flash::getPixelColumn(uint8_t imageNo, uint8_t columnNo,
		uint8_t* spiBuffer1, uint8_t* spiBuffer2, uint8_t* spiBuffer3,
		uint8_t* spiBuffer4) {
	if (debug)

		terminal->sendString("Loading pixel column from flash...\n\r");

	imageNo = imageNo % MaxImageStored;

	uint32_t imageColumnStartPage = FirstImagePageAddress
			+ (imageNo * PagesPerImage);
	uint32_t pixelColumnStartPage = imageColumnStartPage
			+ (floor((columnNo * ColumnPixelArraySize) / PageSize));
	uint32_t pixelColumnStartByte = (columnNo * ColumnPixelArraySize)
			% PageSize;

	if (debug) {
		terminal->sendString("\n\rpixelColumnPageOffset :");
		terminal->sendByte32ToBinaryString(pixelColumnStartPage);
		terminal->sendString("\n\rpixelColumnStartPage :");
		terminal->sendByte32ToBinaryString(imageColumnStartPage);
		terminal->sendString("\n\rpixelColumnStartByte :");
		terminal->sendByte32ToBinaryString(pixelColumnStartByte);
	}

	address_t add;
	add.page = pixelColumnStartPage;
	add.byte = pixelColumnStartByte;

	uint32_t address = 0;
	address = add.page;
	address = address << 9;
	address |= add.byte;

	setCS(true);
	spiTransfer(ContinuousPageRead);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));

	spiTransfer(DummyByte);
	spiTransfer(DummyByte);

	for (uint32_t i = 0; i < SPIBufferSize; i++)
		spiBuffer1[i] = spiTransfer(DummyByte);
	for (uint32_t i = 0; i < SPIBufferSize; i++)
		spiBuffer2[i] = spiTransfer(DummyByte);
	for (uint32_t i = 0; i < SPIBufferSize; i++)
		spiBuffer3[i] = spiTransfer(DummyByte);
	for (uint32_t i = 0; i < SPIBufferSize; i++)
		spiBuffer4[i] = spiTransfer(DummyByte);

	setCS(false);

	if (debug)

		terminal->sendString("Column loaded from flash...\n\r");
	return true;
}

void Flash::getPixelColumnToString(uint8_t imageNo, uint8_t columnNo){

	imageNo = imageNo % MaxImageStored;

	uint32_t imageColumnStartPage = FirstImagePageAddress
			+ (imageNo * PagesPerImage);
	uint32_t pixelColumnStartPage = imageColumnStartPage
			+ (floor((columnNo * ColumnPixelArraySize) / PageSize));
	uint32_t pixelColumnStartByte = (columnNo * ColumnPixelArraySize)
			% PageSize;

	address_t add;
	add.page = pixelColumnStartPage;
	add.byte = pixelColumnStartByte;

	uint32_t address = 0;
	address = add.page;
	address = address << 9;
	address |= add.byte;

	setCS(true);
	spiTransfer(ContinuousPageRead);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));

	spiTransfer(DummyByte);
	spiTransfer(DummyByte);

	terminal->sendString("B1:");
	for (uint32_t i = 0; i < SPIBufferSize; i++)
		terminal->write(spiTransfer(DummyByte));

	terminal->sendString("B2:");
	for (uint32_t i = 0; i < SPIBufferSize; i++)
		terminal->write(spiTransfer(DummyByte));

	terminal->sendString("B3:");
	for (uint32_t i = 0; i < SPIBufferSize; i++)
		terminal->write(spiTransfer(DummyByte));

	terminal->sendString("B4:");
	for (uint32_t i = 0; i < SPIBufferSize; i++)
		terminal->write(spiTransfer(DummyByte));
	setCS(false);

	if (debug)

		terminal->sendString("Column loaded from flash...\n\r");
}

void Flash::setDebug(bool debug) {
	this->debug = debug;
}

uint8_t Flash::countSetBits(uint32_t n) {
	uint8_t count = 0;
	while (n) {
		count += n & 1;
		n >>= 1;
	}
	return count;
}

void Flash::resetImageInCarrousel(uint8_t imageNo) {
	positionOfPresentImages &= ~(1 << (imageNo));
	writeByte(&PositionOfPresentImagesInCarrouselAddress,
			positionOfPresentImages);
}

void Flash::setFilename(uint8_t imageNo, uint8_t* fileName) {
	imageNo = imageNo % MaxImageStored;
	address_t add = FilenamePage;
	add.byte = imageNo * FilenameSize;
	//writeByte(&add, fileName, sizeof(fileName));
	uint16_t nByte = sizeof(fileName);
	uint8_t data;

	if (debug)
		terminal->sendString("Writing byte\n\r\r");

	uint32_t address = 0;
	address = add.page;
	address = address << 9;
	address |= add.byte;

	setCS(true);
	spiTransfer(WrtitePagesThroughBuf2BIE);
	spiTransfer((address & 0x00FF0000) >> 16);
	spiTransfer((address & 0x0000FF00) >> 8);
	spiTransfer((address & 0x000000FF));
	for (uint16_t i = 0; i < nByte; i++) {
		data = (uint8_t) fileName[i];
		spiTransfer(data);
	}
	setCS(false);
	while (!(readStatusRegister() & BusyFlag))
		;
	if (debug)
		terminal->sendString("Done. \n\r\r");
	if (debug)
		terminal->sendString("setFilename\n\r");
}
void Flash::resetFilename(uint8_t imageNo) {
	imageNo = imageNo % MaxImageStored;
	address_t add = FilenamePage;
	add.byte = imageNo * FilenameSize;
	for (uint8_t i = 0; i < FilenameSize; i++) {
		writeByte(&add, DummyByte);
		add.byte++;
	}
	if (debug)
		terminal->sendString("resetFilename\n\r");
}
void Flash::getFilename(uint8_t imageNo, char *destination) {
	imageNo = imageNo % MaxImageStored;
	address_t add = FilenamePage;
	add.byte = imageNo * FilenameSize;
	readByte(&add, destination, (uint32_t) FilenameSize);
//	if (debug)
//		terminal->sendString("getFilename\n\r");
}

void Flash::formatCarrousel() {
	if (debug)

		terminal->sendString("Resetting number of images...\n\r");
	positionOfPresentImages = 0;
	savePositionOfPresentImagesInCarrousel();
	if (debug)
		terminal->sendString("Reset completed...\n\r");

}

uint8_t Flash::getNextFreeImageSlot() {
	uint8_t counter = 0;
	while ((positionOfPresentImages >> counter++) & 0x01)
		;
	if (counter > MaxImageStored)
		return 0;

	return counter - 1;
}

