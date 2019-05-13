/*
 * stm32f4timer.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: rev
 */

#include "stm32f4timer.hpp"

STM32F4Timer::STM32F4Timer(TIM_TypeDef *tmr, uint32_t frequency, uint16_t prescaler, bool interruptEnable) {
	timer = tmr;

	if (tmr == TIM2) {
		timerTickFreq = 100000000 / (prescaler + 1);
		timerPeriod = (timerTickFreq / frequency) - 1;

		TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;

		TIM_BaseInitStructure.TIM_Period = timerPeriod; // PWM_frequency = timer_tick_frequency / (TIM_Period + 1) ** 1MHz**
		TIM_BaseInitStructure.TIM_Prescaler = prescaler; //timer_tick_frequency = Timer_default_frequency / (prescaler_set + 1)
		TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure);

		if (interruptEnable) {
			NVIC_EnableIRQ(TIM2_IRQn);
			NVIC_SetPriority(TIM2_IRQn, 1);
		}
	}
	if (tmr == TIM3) {
		timerTickFreq = 100000000 / (prescaler + 1);
		timerPeriod = (timerTickFreq / frequency) - 1;
		TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;

		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_BaseInitStructure.TIM_Period = timerPeriod; // PWM_frequency = timer_tick_frequency / (TIM_Period + 1) ** 1MHz**
		TIM_BaseInitStructure.TIM_Prescaler = prescaler; //timer_tick_frequency = Timer_default_frequency / (prescaler_set + 1)
		TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM3, &TIM_BaseInitStructure);

		if (interruptEnable) {
			NVIC_InitTypeDef nvicStructure;
			nvicStructure.NVIC_IRQChannel = TIM3_IRQn;
			nvicStructure.NVIC_IRQChannelPreemptionPriority = 3;
			nvicStructure.NVIC_IRQChannelSubPriority = 1;
			nvicStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&nvicStructure);
		}
	}

	if (tmr == TIM4)
	{
		timerTickFreq = 100000000 / (prescaler+1);
		timerPeriod = (timerTickFreq / frequency)-1;
    
		TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;

		TIM_BaseInitStructure.TIM_Period = timerPeriod; // PWM_frequency = timer_tick_frequency / (TIM_Period + 1) ** 1MHz**
		TIM_BaseInitStructure.TIM_Prescaler = prescaler; //timer_tick_frequency = Timer_default_frequency / (prescaler_set + 1)
		TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure);

		if (interruptEnable) {
			NVIC_EnableIRQ(TIM4_IRQn);
			NVIC_SetPriority(TIM4_IRQn, 1);
		}
	}
}

void STM32F4Timer::enableITUpdate() {
	TIM_ITConfig(timer, TIM_IT_Update, ENABLE);
}

void STM32F4Timer::enablePWM(uint8_t ch, uint32_t dutyCycle) {
	stopTimer();

	switch (ch) {
	case 1: {
		TIM_OCInitTypeDef TIM_OCStruct;

		TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCStruct.TIM_Pulse = (((timerPeriod + 1) * dutyCycle) / 100) - 1; // pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
		TIM_OC1Init(timer, &TIM_OCStruct);
		TIM_OC1PreloadConfig(timer, TIM_OCPreload_Enable);
	}
		break;
	case 2: {
		TIM_OCInitTypeDef TIM_OCStruct;

		TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCStruct.TIM_Pulse = (((timerPeriod + 1) * dutyCycle) / 100) - 1; // pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
		TIM_OC2Init(timer, &TIM_OCStruct);
		TIM_OC2PreloadConfig(timer, TIM_OCPreload_Enable);
	}
		break;
	case 3: {
		TIM_OCInitTypeDef TIM_OCStruct;

		TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCStruct.TIM_Pulse = (((timerPeriod + 1) * dutyCycle) / 100) - 1; // pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
		TIM_OC3Init(timer, &TIM_OCStruct);
		TIM_OC3PreloadConfig(timer, TIM_OCPreload_Enable);
	}
		break;
	case 4: {
		TIM_OCInitTypeDef TIM_OCStruct;

		TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCStruct.TIM_Pulse = (((timerPeriod + 1) * dutyCycle) / 100) - 1; // pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
		TIM_OC4Init(timer, &TIM_OCStruct);
		TIM_OC4PreloadConfig(timer, TIM_OCPreload_Enable);
	}
		break;
	}
}

void STM32F4Timer::startTimer() {
	timer->CR1 |= TIM_CR1_CEN;
}

void STM32F4Timer::stopTimer() {
	timer->CR1 &= ~TIM_CR1_CEN;
}
