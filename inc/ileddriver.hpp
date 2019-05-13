/*
 * ileddriver.hpp
 *
 *  Created on: Mar 1, 2019
 *      Author: rev
 */

#ifndef ILEDDRIVER_HPP_
#define ILEDDRIVER_HPP_
#include <stdint-gcc.h>

class ILedDriver {
public:

	virtual ~ILedDriver() {}
	virtual void init() = 0;
	virtual void latch(bool lat) = 0;
	virtual void updateConrol() = 0;
	virtual void updateLeds() = 0;
	virtual void setLedRGB(uint16_t led_number,uint16_t red, uint16_t green, uint16_t blue) = 0;
	virtual void setLedRGB(uint16_t led_number,uint16_t rgb) = 0;
	virtual void setAllLedsRGB(uint16_t red, uint16_t green, uint16_t blue) = 0;
	virtual void setMaxCurrent(uint8_t red, uint8_t green, uint8_t blue) = 0;
	virtual void setMaxCurrent(uint8_t rgb) = 0;
	virtual void setBrightnessCurrent(uint8_t rgb);
	virtual void setBrightnessCurrent(uint8_t red, uint8_t green, uint8_t blue);
};



#endif /* ILEDDRIVER_HPP_ */
