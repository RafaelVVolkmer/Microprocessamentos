#include "stm32f4xx.h"

static uint8_t BordaSubida, BordaDescida;
uint16_t DivisaoCiclo = 0;

//DEBOUNCE + STATE_DETECTION | INTERRUPT FUNCTION
void TIM1_UP_TIM10_IRQHandler ()
{

	TIM10->SR &=~ TIM_SR_UIF;

	typedef enum {ANTERIOR, ATUAL} instantes_t;

	instantes_t Estado[2];

	uint8_t Botao = GPIOB->IDR & GPIO_IDR_IDR_1;

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

			if(Estado[ATUAL] == 0 && Estado[ANTERIOR]==1)
			{
					BordaSubida = 0;
					BordaDescida = 1;
			}

			Estado[ANTERIOR] = Estado[ATUAL];

}

void ManipulacaoBorda ()
{

	if(BordaSubida)
		{

			GPIOC->ODR |= GPIO_ODR_ODR_3;

			if(BordaDescida)
			{
				GPIOC->ODR &=~ GPIO_ODR_ODR_3;
			}

	 	}

}

void ManipulaChaves ()
{

	uint16_t Chaves = GPIOB->IDR & (GPIO_IDR_IDR_0 | GPIO_IDR_IDR_2);

	switch(Chaves)
	{

		case 0b101:

			break;

		case 0b100:

			break;

		case 0b001:

			break;

		default:

			break;

	}

}

void DutyCicle ()
{

	if(TIM11->SR & TIM_SR_UIF)
	{

		TIM11->SR &=~ TIM_SR_UIF;

		DivisaoCiclo ++;

		//25% DutyCicle
			if(DivisaoCiclo == 0 || DivisaoCiclo == 1)
			{
				GPIOC->ODR |= GPIO_ODR_ODR_3;
			}

			if(DivisaoCiclo == 2 || DivisaoCiclo == 3)
			{
				GPIOC->ODR &=~ GPIO_ODR_ODR_3;
			}

			if(DivisaoCiclo == 4)
			{

				DivisaoCiclo = 0;

			}

	}

}



void EsperaParaDesligar ()
{

	if(BordaSubida)
	{

		GPIOC->ODR |= GPIO_ODR_ODR_3;
		TIM11->CNT = 0;

		while((TIM11->SR & TIM_SR_UIF) == 0){};

		GPIOC->ODR &=~ GPIO_ODR_ODR_3;
		TIM11->SR &=~ TIM_SR_UIF;

		BordaSubida = 0;

	}

}

int main(void)
{

	//CLOCK - GPIO
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN| RCC_AHB1ENR_GPIOCEN;

	//CLOCK - TIEMER
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN | RCC_APB2ENR_TIM11EN;


	//CONFIG - PINS: GPIOC 3 (OUTPUT)
		GPIOC->MODER &=~ (GPIO_MODER_MODER3);
		GPIOC->MODER |= (GPIO_MODER_MODER3_0);

	//CONFIG - PINS: GPIOB 0 - 1 - 2 (INPUT)
		GPIOB->MODER &=~ (GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2);

		//PULL - DOWN
		GPIOB->PUPDR &=~ (GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2);
		GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR0_1 | GPIO_PUPDR_PUPDR1_1 | GPIO_PUPDR_PUPDR2_1);


	//CONFIG - TIMER (TIM 10)
		TIM10->PSC = (16000-1);
		TIM10->ARR = (50-1);
		TIM10->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;

		//INTERRUPT - ON
		TIM10->DIER |= TIM_DIER_UIE;

	//CONFIG - INTERRUPT (TIM 10)
		NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
		NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1);

	//CONFIG - TIMER (TIM 11)
		TIM11->PSC = (16000-1);
		TIM11->ARR = (200-1);
		TIM11->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;

		while (1)
		{

			//ManipulacaoBorda ();
			//ManipulaChaves ();
			//EsperaParaDesligar();
			//DutyCicle();

		}
}
