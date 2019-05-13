/*
 * stm32spi1.hpp
 *
 *  Created on: Feb 28, 2019
 *      Author: Zandwich
 */

#ifndef STM32SPI1_HPP_
#define STM32SPI1_HPP_

#include "stm32f4xx_spi.h"
#include "hardware.h"
#include "ispi.hpp"

class STM32SPI1 : public ISPI {
public:

	STM32SPI1();
	~STM32SPI1();
	/**
	 * Initialisation de la classe SPI ainsi que les GPIOs
	 */
	void init();
/**
 * Mise en mode OUTPUT pour les GPIOs de CLK et MOSI afin d'envoyer des données en bit banging
 */
	void setBitBang();
	/**
	 * Envoie d'instruction pour régler le TLC5955 en mode Control
	 */
	void sendControlBits();
	/**
	 * Envoie d'une donnée manuellement sans passer par le SPI à
	 * travers les sorties MOSI et CLK (bit banging)
	 * @param data : byte à envoyer
	 */
	void sendManualByte(uint8_t data);
	/**
	 * Envoie d'une donnée 8bits non-signé par le port SPI
	 * @param data : byte à envoyer
	 */
	void sendByte8(uint8_t data);
	/**
	 * Envoie d'une donnée 16bits non-signé par le port SPI
	 * @param data : byte à envoyer
	 */
	void sendByte16(uint16_t data);
	/**
	 * Réception d'une donnée par le port SPI
	 * @return donnée reçu en 2 bytes non signé (16bits)
	 */
	uint16_t receiveData();
	/**
	 * Activation du chip select (soft)
	 */
	void assert();
	/**
	 * Désactivation du chip select (soft)
	 */
	void deassert();
	/**
	 *
	 * @param spi
	 */

private:
	static constexpr uint8_t ControlDataByte	= 0x96;
};



#endif /* STM32SPI1_HPP_ */
