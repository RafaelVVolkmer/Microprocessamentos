//Rafael V. Volkmer - 4324 / N°21 | Teclado Musical
#include "stm32f4xx.h"

#define FrequenciaPSC (4000-1)
#define FrequenciaARR (1000-1)
#define mascara 0b11111111

uint8_t tempo;
uint8_t teclas;
uint8_t teclas_sustenidas;
volatile uint32_t *nota;

enum {DO=1,RE,MI=4,FA=8,SOL=16,LA=32,SI=64};
enum {DOs=1,REs,MIs=4,FAs=8,SOLs=16,LAs=32,SIs=64};

//Função para ativar saída com o botão pressionado
void chaveamento()
{
	if (teclas)
	{
		if (TIM10->SR & TIM_SR_UIF)
		{
			GPIOA->ODR^=GPIO_ODR_ODR_7;
		}
	}else
	{
		GPIOA->ODR&=~GPIO_ODR_ODR_7;
	}
}

int main(void)
{

	//Ativando clock GPIOA e TIMER10
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

	//PINOS GPIOC - ENTRADAS DAS NOTAS NORMAIS.
	GPIOC->MODER &=~
		(
			GPIO_MODER_MODER0 |
			GPIO_MODER_MODER1 |
			GPIO_MODER_MODER2 |
			GPIO_MODER_MODER3 |
			GPIO_MODER_MODER4 |
			GPIO_MODER_MODER5 |
			GPIO_MODER_MODER6
		);

	GPIOC->MODER |=

			GPIO_MODER_MODER1_0 |
			GPIO_MODER_MODER2_0 |
			GPIO_MODER_MODER3_0 |
			GPIO_MODER_MODER4_0 |
			GPIO_MODER_MODER5_0 |
			GPIO_MODER_MODER6_0 |
			GPIO_MODER_MODER7_0;

	//PINOS GPIOB - ENTRADAS DAS NOTAS SUSTENIDAS.
	GPIOB->MODER &=~
		(
			GPIO_MODER_MODER0 |
			GPIO_MODER_MODER1 |
			GPIO_MODER_MODER2 |
			GPIO_MODER_MODER3 |
			GPIO_MODER_MODER4 |
			GPIO_MODER_MODER5 |
			GPIO_MODER_MODER6
		);

		GPIOB->MODER |=

			GPIO_MODER_MODER1_0 |
			GPIO_MODER_MODER2_0 |
			GPIO_MODER_MODER3_0 |
			GPIO_MODER_MODER4_0 |
			GPIO_MODER_MODER5_0 |
			GPIO_MODER_MODER6_0 |
			GPIO_MODER_MODER7_0;

	//SAÍDA DO SOM - PA7.
	GPIOA->MODER &=~ GPIO_MODER_MODER7;

	//Definindo frequências para o TIMER10
	TIM10->PSC = FrequenciaPSC;
	TIM10->ARR= FrequenciaARR;

	//Habilitando contagem do TIMER10
	TIM10->CR1 = TIM_CR1_CEN|TIM_CR1_ARPE;

	while (1)
	{
		//Mascaramentos
		teclas = GPIOC->IDR & mascara;
		teclas_sustenidas = GPIOB->IDR & mascara;
		tempo = TIM10->SR & TIM_SR_UIF;

		nota = &(TIM10->ARR);



		chaveamento();

		switch(teclas)
		{
			case DO:
				*nota=1; //*calcular frequências em ARR para chegar nas notas
			    	break;

			case RE:
				*nota=1;
		  			break;

			case MI:
				*nota=1;
		  			break;

			case FA:
				*nota=1;
		  			break;

			case SOL:
				*nota=1;
		  			break;

			case LA:
				*nota=1;
		  			break;

			case SI:
				*nota=1;
		  			break;

			default:
				*nota=0;
		  	  		break;
		 }

		 switch(teclas_sustenidas)
		 {
		 	 case DOs:
		   		*nota=1;
		   		    break;

		   	 case REs:
		   		*nota=1;
		   			break;

		   	 case MIs:
		   		*nota=1;
		   			break;

		   	 case FAs:
		   		*nota=1;
		   			break;

		   	 case SOLs:
		   		*nota=1;
		   			break;

		   	 case LAs:
		   		*nota=1;
		   			break;

		   	 case SIs:
		   		*nota=1;
		   			break;

		  	  default:
		  		*nota=0;
		  		  	break;
		 }
	}
}
