/*
 * TMutex.hpp
 *
 *  Created on: 2016-07-28
 *      Author: Alexandre Salconi-Denis
 */

#ifndef CPP_MUTEX_HPP_
#define CPP_MUTEX_HPP_



/**
 * @class TMutex TMutex.hpp
 * @brief Class object for Adaptation from FreeRTOS.
 */
class TMutex
{
public:
	/*virtual*/ TMutex(void){m_mutex=false;};
	virtual ~TMutex(void){};

	virtual bool take(void){return (m_mutex)?false:(m_mutex=true);}

	virtual bool give(void){return m_mutex=false;}

protected:
	bool m_mutex;
};

#endif /* CPP_MUTEX_HPP_ */
