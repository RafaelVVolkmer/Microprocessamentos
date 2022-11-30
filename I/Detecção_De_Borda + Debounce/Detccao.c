#include "stm32f4xx.h"

static uint8_t BordaSubida, BordaDescida;

//EVENT DETECT + DEBOUNCE FUNCTION
void TIM1_UP_TIM10_IRQHandler ()
{
	
	TIM10->SR &=~ TIM_SR_UIF;

	typedef enum {ANTERIOR, ATUAL} estado_t;

	estado_t Estado[2];

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
		RCC->AHB1ENR|= RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOCEN;

	//CLOCK - TIEMER
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

	//CONFIG - PINS | GPIOC - 0 (INPUT)
		GPIOC->MODER &=~ GPIO_MODER_MODER0;
		GPIOC->MODER |= GPIO_MODER_MODER0_0;

	//CONFIG - PINS | GPIOB - 0 (OUTPUT)
		GPIOB->MODER &=~ GPIO_MODER_MODER0;
	
		//CONFIG - GPIOB0 = PULL DOWN
			GPIOB->PUPDR &=~ GPIO_PUPDR_PUPDR0;
			GPIOB->PUPDR |= GPIO_PUPDR_PUPDR0_1;


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
