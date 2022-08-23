//Rafael V. Volkmer - 4324 / N°21 | Prova - Segundo Trimestre (2)
#include "stm32f4xx.h"

//PSC
#define FrequenciaPSC (16000-1)
///ARR (1hz)
#define FrequenciaARR0 (1000-1)
//ARR (2hz)
#define FrequenciaARR1 (500-1)
//ARR (3hz)
#define FrequenciaARR2 (333-1)
//ARR (4hz)
#define FrequenciaARR3 (250-1)

//ARR PONTO
#define FrequenciaARR_PONTO (1000-1)

#define mascara 0b1100000

uint8_t SegundoTempo;
uint8_t PrimeiroTempo;
uint8_t i;
uint16_t teclas;
uint8_t ponto = 0b10000000;

const uint32_t vet[17]=
{
	0b00000000, //null
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111, // 9
        0b01110111, // A
        0b01111100, // B
        0b00111001, // C
        0b01011110, // D
        0b01111001, // E
        0b01110001 // F
};

void Contador ()
{

	GPIOC->ODR &=~ 0b1111111;
	GPIOC->ODR |= vet[i];

	PrimeiroTempo = TIM11->SR & TIM_SR_UIF;

		          if(PrimeiroTempo)
		          {
		              i++;
		              TIM11->SR &=~ TIM_SR_UIF;

		              if(i>16)
		              {
		            	  i = 1;
		              }

		          }
}

void PiscaPonto	()
{
		SegundoTempo = TIM10->SR & TIM_SR_UIF;

			if(SegundoTempo)
			{

				GPIOC->ODR ^= ponto;

				TIM10->SR &=~ TIM_SR_UIF;

			}
}

int main(void)
{

	//Habilitando CLOCKS dos Registradores
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN | RCC_APB2ENR_TIM11EN;

	//PA5 e PA6 - Entradas
	GPIOA->MODER &=~
			(
					GPIO_MODER_MODER6 |
					GPIO_MODER_MODER5
			);

	//PA5 e PA6 - Pull-Down
	GPIOA->PUPDR &=~
			(
					GPIO_PUPDR_PUPDR5 |
					GPIO_PUPDR_PUPDR6
			);

	GPIOA->PUPDR |=
					GPIO_PUPDR_PUPDR5_1 |
					GPIO_PUPDR_PUPDR6_1;

	// PC0 a PC8 - Saídas
	GPIOC->MODER &=~
			(
					GPIO_MODER_MODER0  |
					GPIO_MODER_MODER1  |
					GPIO_MODER_MODER2  |
					GPIO_MODER_MODER3  |
					GPIO_MODER_MODER4  |
					GPIO_MODER_MODER5  |
					GPIO_MODER_MODER6  |
					GPIO_MODER_MODER7
			);

	GPIOC->MODER |=
					GPIO_MODER_MODER0_0  |
					GPIO_MODER_MODER1_0  |
					GPIO_MODER_MODER2_0  |
					GPIO_MODER_MODER3_0  |
					GPIO_MODER_MODER4_0  |
					GPIO_MODER_MODER5_0  |
					GPIO_MODER_MODER6_0  |
					GPIO_MODER_MODER7_0;

	//configurações TIMER11
	TIM11->PSC = FrequenciaPSC;

		TIM11->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;

	//configurações TIMER10
	TIM10->PSC = FrequenciaPSC;
	TIM10->ARR = FrequenciaARR_PONTO;

		TIM10->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;

while (1)
{

	Contador ();

	PiscaPonto ();

	          teclas = GPIOA->IDR & mascara;
	          	  teclas = teclas >> 5;

	          switch(teclas)
	          {
	          	  case 0:
	          		  TIM11->ARR = FrequenciaARR0;
	          		  	  break;
	          	  case 1:
	          		  TIM11->ARR = FrequenciaARR1;
	          		  	  break;
	          	  case 2:
	          		  TIM11->ARR = FrequenciaARR2;
	          		  	  break;
	          case 3:
	              	  TIM11->ARR = FrequenciaARR3;
	              	  	  break;
	          }

	      }

}
