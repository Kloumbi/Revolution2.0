#include "Stm32f446Timer.h"

Stm32f446Timer::Stm32f446Timer(TIM_TypeDef * tmr, uint32_t us,bool interruptEnable)
{
	timer = tmr;
	if ( tmr == TIM2)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
		if(interruptEnable)
		{
			NVIC_EnableIRQ(TIM2_IRQn);
			NVIC_SetPriority(TIM2_IRQn,1);
		}
	}
	if ( tmr == TIM3)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		if(interruptEnable)
		{
		NVIC_EnableIRQ(TIM3_IRQn);
		NVIC_SetPriority(TIM3_IRQn,1);
		}
	}
	if ( tmr == TIM4)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
		if(interruptEnable)
		{
		NVIC_EnableIRQ(TIM4_IRQn);
		NVIC_SetPriority(TIM4_IRQn,0);
		}
	}
	if ( tmr == TIM5)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
		if(interruptEnable)
		{
		NVIC_EnableIRQ(TIM5_IRQn);
		NVIC_SetPriority(TIM5_IRQn,2);
		}
	}
	setPeriod(us);
	if(interruptEnable)
		timer->DIER = TIM_DIER_UIE;
	timer->CR1 |= TIM_CR1_ARPE;
	start();
}
void Stm32f446Timer::setPeriod(uint32_t us)
{
	uint32_t divFactor = 2000000;
	uint32_t reload = us - 1;

	if ( us > 10000)
	{
		reload = us / 100 - 1;
		divFactor = 20000;
	}

	if(reload < 1)
		reload = 1;

	stop();
	timer->PSC = SystemCoreClock / divFactor - 1;
	timer->ARR = reload;
}
void Stm32f446Timer::enablePWM(uint8_t ch, uint32_t freq, uint16_t range)
{
	timer->CR1 |= TIM_CR1_DIR;
	if((range+1) * freq * 2 > SystemCoreClock)
	{
		freq = SystemCoreClock / ( 2 * (range+1) );
	}
	timer->ARR = range;
	timer->PSC = SystemCoreClock / (freq*2*range) - 1;

	switch(ch)
		{
		case 1  :
			timer->CCMR1 &= ~TIM_CCMR1_OC1M;
			timer->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
			timer->CCR1 = range>>1;
			timer->CCER |= TIM_CCER_CC1E;
			break;
		case 2  :
			timer->CCMR1 &= ~TIM_CCMR1_OC2M;
			timer->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
			timer->CCR2 = range>>1;
			timer->CCER |= TIM_CCER_CC2E;
			break;
		case 3  :
			timer->CCMR2 &= ~TIM_CCMR2_OC3M;
			timer->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;
			timer->CCR3 = range>>1;
			timer->CCER |= TIM_CCER_CC3E;
			break;
		case 4  :
			timer->CCMR2 &= ~TIM_CCMR2_OC4M;
			timer->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
			timer->CCR4 = range>>1;
			timer->CCER |= TIM_CCER_CC4E;
			break;
		}
}
void Stm32f446Timer::setPWMLvl(uint8_t ch, uint16_t lvl)
{
	switch(ch)
	{
	case 1:
		if(lvl == 0)
			timer->CCMR1 &= ~TIM_CCMR1_OC1M_1;
		else
			timer->CCMR1 |= TIM_CCMR1_OC1M_1;
		timer->CCR1 = lvl;
		break;
	case 2:
		if(lvl == 0)
			timer->CCMR1 &= ~TIM_CCMR1_OC2M_1;
		else
			timer->CCMR1 |= TIM_CCMR1_OC2M_1;
		timer->CCR2 = lvl;
		break;
	case 3:
		if(lvl == 0)
			timer->CCMR2 &= ~TIM_CCMR2_OC3M_1;
		else
			timer->CCMR2 |= TIM_CCMR2_OC3M_1;
		timer->CCR3 = lvl;
		break;
	case 4:
		if(lvl == 0)
			timer->CCMR2 &= ~TIM_CCMR2_OC4M_1;
		else
			timer->CCMR2 |= TIM_CCMR2_OC4M_1;
		timer->CCR4 = lvl;
		break;
	}
}

void Stm32f446Timer::start()
{
	timer->CR1 |= TIM_CR1_CEN;
}

void Stm32f446Timer::stop()
{
	timer->CR1 &= ~TIM_CR1_CEN;
}
