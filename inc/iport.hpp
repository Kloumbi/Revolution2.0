/*
 * iport.hpp
 *
 *  Created on: Feb 28, 2019
 *      Author: Zandwich
 */

#ifndef IPORT_HPP_
#define IPORT_HPP_

class IPort
{
public:

	virtual ~IPort();
	virtual void openPort(){};
	virtual  void closePort(){};
};



#endif /* IPORT_HPP_ */
