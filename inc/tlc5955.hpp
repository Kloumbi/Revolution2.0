/*
 * tlc5955.hpp
 *
 *  Created on: Mar 1, 2019
 *      Author: Zandwich
 */

#ifndef TLC5955_HPP_
#define TLC5955_HPP_

#include "ileddriver.hpp"
#include "hardware.h"

#include "stm32spi1.hpp"
#include "stm32spi2.hpp"
#include "stm32spi3.hpp"
#include "stm32spi4.hpp"

/* Bit Quantities (Change to match other TLC driver chips) */
#define GS_BITS 16
#define GB_BITS 7
#define MC_BITS 3
#define FC_BITS 5
#define DC_BITS 7
#define CONTROL_ZERO_BITS 389   /* Bits required for correct control reg size */
#define TOTAL_REGISTER_SIZE 769
#define LATCH_DELAY 10
#define CONTROL_WRITE_COUNT 2
#define CONTROL_MODE_ON 1
#define CONTROL_MODE_OFF 0

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

// LED Current OUTPUT
static const float LED_CURRENT_AMPS = 0.020;

// Line ending for serial output
static const char LINE_ENDING[] = "\n";

class TLC5955 {
public:
	TLC5955();
	~TLC5955();

	//-------------Initialization---------------------------------------------------------------------------------
	void init(STM32SPI1 *spi_1, STM32SPI2 *spi_2, STM32SPI3 *spi_3, STM32SPI4 *spi_4);


	//-------------Send data to device(update, flush,latch)-------------------------------------------------------
	void updateControl();
	void updateLeds(uint8_t *buffer1,uint8_t *buffer2,uint8_t *buffer3,uint8_t *buffer4);
	void latch(bool lat);
	void setBuffer(uint8_t bit);
	void setControlModeBit(bool isControlMode);
	void flushBuffer();
	void resetAllLatch();

	//-------------Set LEDS intensities---------------------------------------------------------------------------
	void setLedRGB(uint16_t led_number,uint16_t red, uint16_t green, uint16_t blue);
	void setLedRGB(uint16_t led_number,uint16_t rgb);
	void setAllLedsRGB(uint16_t red, uint16_t green, uint16_t blue);
	void setAllLed(uint16_t gsvalue);
	void setLedAppend(uint16_t led_number, uint16_t red, uint16_t green, uint16_t blue);
	void setChannel(uint16_t channel_number, uint16_t value);

	//-------------Control Mode Parameters------------------------------------------------------------------------
	void setLedDotCorrection(uint16_t led_number, uint8_t color_channel_index, uint8_t dc_value);
	void setAllDcData(uint8_t dcvalue);
	void setMaxCurrent(uint8_t red, uint8_t green, uint8_t blue);
	void setMaxCurrent(uint8_t MCRGB);
	void setBrightnessCurrent(uint8_t rgb);
	void setBrightnessCurrent(uint8_t red, uint8_t green, uint8_t blue);
	void setFunctionControlData(bool DSPRPT, bool TMGRST, bool RFRESH, bool ESPWM, bool LSDVLT);
	void setRGBPinOrder(uint8_t red_pos, uint8_t green_pos, uint8_t blue_pos);
	void setSinglePinOrder(uint16_t led_number, uint8_t color_channel_index, uint8_t position);
	void setSingleRGBPinOrder(uint16_t led_number, uint8_t red_pos, uint8_t green_pos, uint8_t blue_pos);

	//-------------SPI FUNCTIONS----------------------------------------------------------------------------------
	void deassertAll();
	void assertAll();
	void setBitBangConfig();

	//-------------OTHER FUNCTIONS--------------------------------------------------------------------------------
	void setPixelMap(uint16_t lednumber,uint16_t red, uint16_t green, uint16_t blue, uint8_t *buffer1,uint8_t *buffer2,uint8_t *buffer3,uint8_t *buffer4);
	static const uint8_t tlc_count = 12; // This
	static const uint8_t COLOR_CHANNEL_COUNT = 3;
	static const uint8_t LEDS_PER_CHIP = 16;
	static const uint8_t SPI_COUNT = 4;
	static bool force_max_current;
	static float max_current_amps;

	uint8_t dot_correction_data[tlc_count][LEDS_PER_CHIP][COLOR_CHANNEL_COUNT];
	uint8_t rgb_order[tlc_count][LEDS_PER_CHIP][COLOR_CHANNEL_COUNT];
	uint16_t grayscale_data[tlc_count][LEDS_PER_CHIP][COLOR_CHANNEL_COUNT];

	uint8_t rgb_order_default[3] = {0, 1, 2};

private:
	int debug = 0;
	//uint8_t _gslat;
	//uint8_t _spi_mosi;
	//uint8_t _spi_clk;

	uint8_t _function_data;
	uint16_t _bright_red;
	uint16_t _bright_green;
	uint16_t _bright_blue;
	uint8_t _Max_Current_Red = 127;
	uint8_t _Max_Current_Green = 127;
	uint8_t _Max_Current_Blue = 127;

	/* SPI */
	uint8_t _buffer;
	int8_t _buffer_count = 7;
	STM32SPI1 *spi1;
	STM32SPI2 *spi2;
	STM32SPI3 *spi3;
	STM32SPI4 *spi4;
};



#endif /* TLC5955_HPP_ */
