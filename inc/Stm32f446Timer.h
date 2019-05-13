/**
 * @file 	STM32F446Timer.h

 * @brief	Classe générique pour configurer un timer du STM32F446
 * 			Implément aussi les méthodes pour gérer le PWM
 * 			Supporte actuellelement Timer 2 à 5
 * @author  Marc.Juneau
 * @version	0.01
 * @date	15 mars 2019
 * @warning	Vous devez définir le gestionnaire d'interruption dans votre code
 * @section Exemple Exemple de gestionnaire d'interruption pour le timer 2
 * @code
  extern "C" void TIM2_IRQHandler(void)
  {
	if (TIM2->SR & TIM_SR_UIF) // if UIF flag is set
	{
		TIM2->SR &= ~TIM_SR_UIF; // clear UIF flag
		// Votre traitement ici
	}
  }
 * @endcode
 */
//	//Initalise the Timer Modules
//	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
//
//	TIM_BaseInitStructure.TIM_Period = 4900;  // PWM_frequency = timer_tick_frequency / (TIM_Period + 1) ** 1MHz**
//	TIM_BaseInitStructure.TIM_Prescaler = 0;  //timer_tick_frequency = Timer_default_frequency / (prescaler_set + 1)
//	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure);
//
//	TIM_OCInitTypeDef TIM_OCStruct;
//
//	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
//	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCStruct.TIM_Pulse = 2400;// pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
//	TIM_OC1Init(TIM2, &TIM_OCStruct);
//	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
//
//
//	TIM2->CR1 |= TIM_CR1_ARPE;
//
//	TIM_Cmd(TIM2,ENABLE);

#ifndef STM32F446TIMER_H_
#define STM32F446TIMER_H_

#include "stm32f4xx.h"
#include "TimerWithPWM.h"
/**
 * @brief Classe générique pour configurer un timer du STM32F446
 *  Supporte actuellelement Timer 2 à 5
 * @warning	Vous devez définir le gestionnaire d'interruption dans votre code
 */
class Stm32f446Timer : public TimerWithPWM
{
	TIM_TypeDef * timer;
public:
	/**
	 * Constructeur du timer
	 * @param Le timer (Exemple TIM2 pour le timer2)
	 * @param La période du timer en us (de 2 à 65535000 us)
	 */
	Stm32f446Timer(TIM_TypeDef * tmr, uint32_t us,bool interruptEnable);
	/**
	 * Destructeur
	 */
	virtual ~Stm32f446Timer(){}
	/**
	 * Configure la période du timer en us
	 * @param La période du timer en us (de 2 à 65535000 us)
	 */
	void setPeriod(uint32_t us);
	/**
	 * Active le PWM sur un canal avec une fréquence et une étendue
	 * Il faudra configurer le GPIO pour avoir le signal en sortie
	 * @param ch Le canal visé
	 * @param freq La fréquence du PWM en Hz.(Défaut à 10kHz)
	 * @param range Étentue du PWM. (Défaut à 255).
	 */
	void enablePWM(uint8_t ch, uint32_t freq=10000, uint16_t range = 255);
	/**
	 * Ajuste la durée à haut du signal PWM
	 * @param ch Canal visé
	 * @param lvl La durée du niveau haut.
	 *        Pour un duty cycle de 50% sur une étendue de 8 bits (255) lvl = 128.
	 */
	void setPWMLvl(uint8_t ch, uint16_t lvl);
	/**
	 * Démarre le timer
	 */
	void start();
	/**
	 * Arrêt du timer
	 */
	void stop();
};

#endif /* STM32F446TIMER_H_ */
