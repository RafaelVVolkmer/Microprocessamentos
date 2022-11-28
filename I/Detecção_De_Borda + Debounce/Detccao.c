#include "stm32f4xx.h"

static uint8_t BordaSubida, BordaDescida;

void TIM1_UP_TIM10_IRQHandler ()
{

	TIM10->SR &=~ TIM_SR_UIF;

	typedef enum {ANTERIOR, ATUAL} instantes_t;

	instantes_t Estado[2];

	uint8_t Botao = GPIOB->IDR & GPIO_IDR_IDR_0;

		if(Botao)
		{
			Estado[ATUAL] = 1;
		}
		else
		{
			Estado[ATUAL] = 0;
		}

			if(Estado[ATUAL]==1 && Estado[ANTERIOR]==0)
			{
					BordaSubida = 1;
					BordaDescida = 0;
			}

				if(Estado[ATUAL]==0 && Estado[ANTERIOR]==1)
				{
						BordaSubida = 0;
						BordaDescida = 1;
				}

				Estado[ANTERIOR] = Estado[ATUAL];

}

int main(void)
{

	//CLOCK - GPIO
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOCEN|RCC_AHB1ENR_GPIODEN|RCC_AHB1ENR_GPIOHEN;

	//CLOCK - TIEMER
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN | RCC_APB2ENR_TIM11EN;


	//CONFIG - PINS
		GPIOC->MODER &=~ (GPIO_MODER_MODER0);
		GPIOC->MODER |= (GPIO_MODER_MODER0_0);

		GPIOB->MODER &=~ (GPIO_MODER_MODER0 | GPIO_MODER_MODER0);
		GPIOB->PUPDR &=~ (GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR0);
		GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR0_1 | GPIO_PUPDR_PUPDR0_1);


	//CONFIG - TIMER
		TIM10->PSC = (16000-1);
		TIM10->ARR = (50-1);
		TIM10->CR1 = TIM_CR1_CEN;
		TIM10->DIER |= TIM_DIER_UIE;

	//CONFIG - INTERRUPT
		NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
		NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1);

while (1)
{

	if(BordaSubida)
	{

		//Executar o código aqui

 	}
}
}
