//Trabalho Multiplexação - Segundo Trimestre - MICROS I.  21/09/2022
//Feito Por: Rafael V. Volkmer (Nº21) e Pedro Trento (Nº20)

#include "stm32f4xx.h"
#include "TecladoMatricial.h"

#define Frequencia_PSC 159
#define Frequencia_ARR 99

int main(void)
{


	//Habilitando Clocks
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

	//Entradas
		GPIOB->MODER &=~
				(
						GPIO_MODER_MODER0 |
						GPIO_MODER_MODER1 |
						GPIO_MODER_MODER2 |
						GPIO_MODER_MODER3
				);

	//Entradas PULL- UP
		GPIOB->PUPDR &=~
				(
						GPIO_PUPDR_PUPDR0 |
						GPIO_PUPDR_PUPDR1 |
						GPIO_PUPDR_PUPDR2 |
						GPIO_PUPDR_PUPDR3
				);

		GPIOB->PUPDR |=
						GPIO_PUPDR_PUPDR0_0 |
						GPIO_PUPDR_PUPDR1_0 |
						GPIO_PUPDR_PUPDR2_0 |
						GPIO_PUPDR_PUPDR3_0;

	//Saídas
		GPIOB->MODER &=~
				(
						GPIO_MODER_MODER4 |
						GPIO_MODER_MODER5 |
						GPIO_MODER_MODER6 |
						GPIO_MODER_MODER7
				);

		GPIOB->MODER |=
						GPIO_MODER_MODER4_0 |
						GPIO_MODER_MODER5_0 |
						GPIO_MODER_MODER6_0 |
						GPIO_MODER_MODER7_0;

	//Saídas OPEN - DRAIN
		GPIOB->OTYPER |=
						GPIO_OTYPER_OT_4 |
						GPIO_OTYPER_OT_5 |
						GPIO_OTYPER_OT_6 |
						GPIO_OTYPER_OT_7;

	//Configuração de TIMER
		TIM10->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;

		TIM10->PSC = Frequencia_PSC;
		TIM10->ARR = Frequencia_ARR;

  while (1)
  {

	  TecladoMatricial_Completo ();

	  /*
	    	 A função acima lê ela e retorna o valor pressionado nos botões, a partir da variável Tecla
	   	   	 A variável Tecla já está inclusa dentro da biblioteca
	   	   	 *Manipular variável Tecla
	   */

  }

}
