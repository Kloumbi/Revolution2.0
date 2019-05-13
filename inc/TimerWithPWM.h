/**
 * @file 	TimerWithPWM
 * @brief	Interface pour les timers avec PWM. Dérive l'interface Timer
 * @author  Marc.Juneau
 * @version	0.01
 * @date	1 mars 2019
 */
#ifndef TIMERWITHPWM_H_
#define TIMERWITHPWM_H_
#include "Timer.h"
/**
 * @brief Interface pour les timers avec PWM
 */
class TimerWithPWM : public Timer
{
public:
	/**
	 * Destructeur
	 */
	virtual ~TimerWithPWM(){}
	/**
	 * Active le PWM sur un canal.
	 * Il faudra configurer le GPIO pour avoir le signal en sortie
	 * @param ch Le canal visé
	 * @param freq La fréquence du PWM en Hz.
	 * @param range Étentue du PWM. Exemple 255 pour une étentue de 8 bits.
	 */
	virtual void enablePWM(uint8_t ch, uint32_t freq, uint16_t range)=0;
	/**
	 * Ajuste la durée à haut du signal PWM
	 * @param ch Canal visé
	 * @param lvl La durée du niveau haut.
	 *        Pour un duty cycle de 50% sur une étendue de 8 bits (255) lvl = 128.
	 */
	virtual void setPWMLvl(uint8_t ch, uint16_t lvl)=0;
};

#endif
