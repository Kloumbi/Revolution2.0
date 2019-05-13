/*
 * ispi.hpp
 *
 *  Created on: Feb 28, 2019
 *      Author: rev
 */

#ifndef ISPI_HPP_
#define ISPI_HPP_


class ISPI
{
public:

	virtual ~ISPI(){}
	virtual void init() =0;

	virtual void sendByte8(uint8_t data) =0;
	virtual uint16_t receiveData() =0;

	virtual void assert() =0;
	virtual void deassert() =0;
};




#endif /* ISPI_HPP_ */
