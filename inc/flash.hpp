/*
 * flash.hpp
 *
 *  Created on: Feb 28, 2019
 *      Author: Kloumbi
 */

#ifndef FLASH_HPP_
#define FLASH_HPP_

#include "stm32f4xx_spi.h"
#include "hardware.h"
#include "ispi.hpp"
#include <stdio.h>

class Flash {
public:
	uint32_t positionOfPresentImages = 0;
	uint8_t redMaxCurrent = 127;
	uint8_t greenMaxCurrent = 127;
	uint8_t blueMaxCurrent = 127;
	uint8_t globalBrightness = 127;


	struct address_t {
		uint16_t page = 0;
		uint16_t byte = 0;
	};

	static Flash* getInstance(bool debug = 0);
	static constexpr uint8_t MaxImageStored = 14;
	static constexpr uint16_t FilenameSize = 16;					//in bytes
	static constexpr uint16_t SPIBufferSize = 289;					//in bytes
	static constexpr uint16_t ColumnPixelArraySize = 1156;			//in bytes
	static constexpr uint32_t ImageFileSize = 295936;				//in bytes

	static constexpr uint16_t PagesPerImage = 578;					//Flash memory pages per image
	static constexpr uint8_t FirstImagePageAddress = 99;			//First age where image are stored
	static constexpr uint16_t PageSize = 512;						//Physical flash memory page size in bytes
	static constexpr uint16_t MaxColumnCount = 256;



	~Flash();
	/**
	 * page : page number 0 - 8191
	 * byte : byte number 0 -255
	 */

	/**
	 * Initialize SPI port
	 */
	void init();
	/**
	 *
	 * @param data : byte to be transfered via SPI
	 * @return byte read from SPI Data Register
	 */
	uint8_t spiTransfer(uint8_t data);
	/**
	 *
	 * @param state : 1 = assert Chip Select 0 = deassert Chip Select
	 */
	void setCS(bool state);
	/**
	 *
	 * @return busy state. 0 = chip ready for operation
	 */
	bool isBusy();
	/**
	 *
	 * @return returning 2bytes containing the status register
	 */
	uint16_t readStatusRegister();
	/**
	 *
	 * @return byte containing the configuration register
	 */
	uint8_t readConfigurationRegister();

	void readStatusRegisterToString();

	uint8_t readByte(const address_t *add);
	void writeByte(const address_t *add, uint8_t byte);
	void writeByte(const address_t *add, int byte);
	void writeByte(const address_t *add, uint16_t byte);
	void writeByte(const address_t *add, uint32_t byte);
	void writeByte(const address_t *add, uint8_t *byte, uint16_t nByte, uint16_t offsetByte = 0);
	void writeByte(const address_t *add, const char *byte, uint16_t nByte);

	void readByte(const address_t *add, uint8_t *buffer, uint16_t nBytes);
	void readByte(const address_t *add, char *buffer, uint16_t nBytes);
	void readPageArray(const address_t *add, uint8_t *buffer, uint32_t nByte);
	void erasePage(const address_t *add);
	void eraseChip();
	void disableSectorProtection();
	void getDeviceID(uint8_t *buffer);
	void setPageSizeBinary();

	void readControlRegister();
	void writeControlRegister();

	uint32_t getPositionOfPresentImagesInCarrousel();
	void savePositionOfPresentImagesInCarrousel();

	void setImageInCarrousel(uint8_t imageNo);
	void resetImageInCarrousel(uint8_t imageNo);
	void formatCarrousel();

	uint8_t getNumberOfImagesInCarrousel();
	uint8_t getNextFreeImageSlot();

	void getFilename(uint8_t imageNo, char *destination);
	void setFilename(uint8_t imageNo, uint8_t *fileName);
	void resetFilename(uint8_t imageNo);

	bool getPixelColumn(uint8_t imageNo, uint8_t columnNo, uint8_t* spiBuffer1, uint8_t* spiBuffer2, uint8_t* spiBuffer3, uint8_t* spiBuffer4);
	bool savePixelColumn(uint8_t imageNo, uint8_t columnNo, uint8_t* source);
	void getPixelColumnToString(uint8_t imageNo, uint8_t columnNo);
	void setDebug(bool debug);
	uint8_t countSetBits(uint32_t n);

private:
	Flash();
	Flash(bool debugEnable);

	static Flash* instance;


	/**
	 * Flash read/write and SPI command
	 */

	static constexpr uint16_t BusyFlag = 0x8080;
	static constexpr uint16_t ReadWriteErrorFlag = 0x0020;

	static constexpr uint8_t DummyByte = 0x00;

	static constexpr uint8_t MainMemmoryPageRead = 0xD2;
	static constexpr uint8_t ContinuousPageRead = 0x1B;
	static constexpr uint8_t WrtitePagesThroughBuf1BIE = 0x82;
	static constexpr uint8_t WrtitePagesThroughBuf2BIE = 0x85;
	static constexpr uint8_t WrtiteBytesThroughBuf1NoBIE = 0x02;
	static constexpr uint8_t PageErase = 0x81;
	static constexpr uint8_t DeviceID = 0x9F;
	static constexpr uint8_t StatusRegisterRead = 0xD7;
	static constexpr uint8_t ConfigurationRegisterRead = 0x3F;

	static constexpr uint8_t BinaryPageSize[4] = { 0x3D, 0x2A, 0x80, 0xA6 };
	static constexpr uint8_t ChipErase[4] = { 0xC7, 0x94, 0x80, 0x9A };
	static constexpr uint8_t DisableSectorProtect[4] = {0x3D, 0x2A, 0x7F, 0x9A};

	/**
	 * Static configuration addresses
	 */

	/*
	 * Page 9 is reserved for BMP file names: 15 * 16 char = 240 bytes ex:123456789012.bmp
	 */

	const address_t FilenamePage = { 9, 0 };								//15 images * 34 char = 510 bytes
	const address_t GlobalBrightnessSettingAddress = { 10, 0 };				//2 bytes
	const address_t RedMaxCurrentSettingAddress = { 10, 2 };				//2 bytes
	const address_t GreenMaxCurrentSettingAddress = { 10, 4 };				//2 bytes
	const address_t BlueMaxCurrentSettingAddress = { 10, 6 };				//2 Bytes
	const address_t PositionOfPresentImagesInCarrouselAddress = { 10, 7 };	//4 bytes

	bool buff1_Buff2 = true;
	bool debug = false;


};

#endif /* FLASH_HPP_ */
