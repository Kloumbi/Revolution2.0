/**
 * @file 	Timer.h
 * @brief	Interface pour les timers
 * @author  Marc.Juneau
 * @version	0.01
 * @date	1 mars 2019
 */
#ifndef TIMER_H_
#define TIMER_H_
#include <stdint-gcc.h>
/**
 * @brief Interface pour les timers
 */
class Timer
{
public:
	/**
	 * Destructeur
	 */
	virtual ~Timer(){}
	/**
	 * Démarrage du timer
	 */
	virtual void start() =0;
	/**
	 * Arrêt du timer
	 */
	virtual void stop() =0;
	/**
	 * Configure la periode du timer. Il faut normalement redémarrer le timer apres
	 * @param us Période en us
	 */
	virtual void setPeriod(uint32_t us) =0;
};

#endif /* TIMER_H_ */
