//Rafael V. Volkmer - 4324 / N°21 | Teclado Musical
#include "stm32f4xx.h"
#include <math.h>

#define FrequenciaPSC (1000-1)
#define FrequenciaARR (160-1)
#define mascara 0b111111111111

volatile uint8_t tempo;
volatile uint16_t teclas;
volatile uint32_t *nota;

enum {DO=1, DOs=2, RE=4, REs=8, MI=16, FA=32, FAs=64, SOL=128, SOLs=256, LA=512, LAs=1024, SI=2048};

//Função para ativar saída com o botão pressionado
void chaveamento()
{
	teclas = GPIOC->IDR & mascara;
	tempo = TIM10->SR & TIM_SR_UIF;
	
	if(teclas)
	{
		if(tempo)
		{
			TIM10->SR =~ TIM_SR_UIF;
			GPIOA->ODR^=GPIO_ODR_ODR_7;
		}
	}
}

int frequenciaNota(int nota)
{
	const float constanteNotas = 1.0594631;
	const float doPrimeiraEscala = 261.626;

	const int frequenciaNucleo = 16000000;
	int frequenciaNota, arr;

	frequenciaNota = doPrimeiraEscala*pow(constanteNotas, nota-1);

	arr = (frequenciaNucleo/FrequenciaPSC)/frequenciaNota;

	return arr;
}

//

int main(void)
{

	//Ativando clock GPIOA e TIMER10
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

	//PINOS GPIOC - ENTRADA DAS NOTAS
	GPIOC->MODER &=~
		(
			GPIO_MODER_MODER0  |
			GPIO_MODER_MODER1  |
			GPIO_MODER_MODER2  |
			GPIO_MODER_MODER3  |
			GPIO_MODER_MODER4  |
			GPIO_MODER_MODER5  |
			GPIO_MODER_MODER6  |
			GPIO_MODER_MODER7  |
			GPIO_MODER_MODER8  |
			GPIO_MODER_MODER9  |
			GPIO_MODER_MODER10 |
			GPIO_MODER_MODER11
		);

	// DEFININDO PULL DOWN GPIOC
	GPIOC->PUPDR |=
		(
			GPIO_PUPDR_PUPDR0_1  |
			GPIO_PUPDR_PUPDR1_1  |
			GPIO_PUPDR_PUPDR2_1  |
			GPIO_PUPDR_PUPDR3_1  |
			GPIO_PUPDR_PUPDR4_1  |
			GPIO_PUPDR_PUPDR5_1  |
			GPIO_PUPDR_PUPDR6_1  |
			GPIO_PUPDR_PUPDR7_1  |
			GPIO_PUPDR_PUPDR8_1  |
			GPIO_PUPDR_PUPDR9_1  |
			GPIO_PUPDR_PUPDR10_1 |
			GPIO_PUPDR_PUPDR11_1
		);

	// PINO PA7 - SAÍDA DO SOM
	GPIOA->MODER |= GPIO_MODER_MODER7_0;

	//Definindo frequências para o TIMER10
	TIM10->PSC = FrequenciaPSC;
	TIM10->ARR = FrequenciaARR;

	//Habilitando contagem do TIMER10
	TIM10->CR1 = TIM_CR1_CEN|TIM_CR1_ARPE;

	while (1)
	{
		//Mascaramentos
		teclas = GPIOC->IDR & mascara;

		nota = &(TIM10->ARR);

		chaveamento();

		switch(teclas)
		{
			case DO:
				*nota=frequenciaNota(1);
			    	break;

			case DOs:
				*nota=frequenciaNota(2);
		  			break;

			case RE:
				*nota=frequenciaNota(3);
		  			break;

			case REs:
				*nota=frequenciaNota(4);
		  			break;

			case MI:
				*nota=frequenciaNota(5);
		  			break;

			case FA:
				*nota=frequenciaNota(6);
		  			break;

			case FAs:
				*nota=frequenciaNota(7);
		  			break;

			case SOL:
				*nota=frequenciaNota(8);
		  			break;

			case SOLs:
				*nota=frequenciaNota(9);
		  			break;

			case LA:
				*nota=frequenciaNota(10);
		  			break;

			case LAs:
				*nota=frequenciaNota(11);
		  			break;

			case SI:
				*nota=frequenciaNota(12);
		  			break;
		 }
	}
}
