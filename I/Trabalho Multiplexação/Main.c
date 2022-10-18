//Trabalho Multiplexação - Segundo Trimestre - MICROS I.  21/09/2022
//Feito Por: Rafael V. Volkmer (Nº21) e Pedro Trento (Nº20)

#include "stm32f4xx.h"
#include "TecladoMatricial.h"
#include "Comandos.h"

#define Frequencia_ARR_1s (500-1)
#define Frequencia_PSC_1s (16000-1)

#define Frequencia_ARR_Retina (100-1)
#define Frequencia_PSC_Retina (15-1)

int main(void)
{

	//Habilitando Clocks
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN | RCC_APB2ENR_TIM11EN;

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

	//Saídas Teclado
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

	//Saídas LEDS
		GPIOC->MODER &=~
				(
						GPIO_MODER_MODER0 |
						GPIO_MODER_MODER1 |
						GPIO_MODER_MODER2 |
						GPIO_MODER_MODER3 |
						GPIO_MODER_MODER4 |
						GPIO_MODER_MODER5
				);

		GPIOC->MODER |=
						GPIO_MODER_MODER0_0 |
						GPIO_MODER_MODER1_0 |
						GPIO_MODER_MODER2_0 |
						GPIO_MODER_MODER3_0 |
						GPIO_MODER_MODER4_0 |
						GPIO_MODER_MODER5_0;

	//Led RGB de Controle de Estado
		GPIOA->MODER &=~ (GPIO_MODER_MODER0 | GPIO_MODER_MODER1);

		GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0;

	//Configuações do Timer 10
		TIM10->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;

		TIM10->ARR = Frequencia_ARR_1s;
		TIM10->PSC = Frequencia_PSC_1s;

	//Configuações do Timer 11
		TIM11->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;

		TIM11->ARR = Frequencia_ARR_Retina;
		TIM11->PSC = Frequencia_PSC_Retina;

  while (1)
  {

	  MatrizLeds();

	  TeclaPressionada();

	  ChamaFechadura();

	    switch (Operacao)
	    {

	    	case Entra_Senha:

	    		InserirSenha();

	    			break;

	    	case Compara:

	    		Comparacao();

	    			break;

	    	case Senha_Certa:

	    	  	AbrirArmario();

	    			break;

	    	case Senha_Errada:

	    	  	ErrouSenha();

	    			break;

	    	case Fecha_Armario:

	    		FecharArmario();

	    			break;

	    }

  }
}
