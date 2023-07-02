//Conecte um display de 7 segmentos aos pinos do arm, junto com 2 chaves
// - Se nenhuma chave estiver pressionada, o display deve exibir a contagem de 0 a 5 em uma frequencia de 200hz, e o ponto desligado
// - Se a primeira chave estiver pressionada, o ponto deve piscar a uma determinada frequencia
// - Se a segunda chave estiver pressionada,  o display deve exibir a contagem de 0 a 5 em uma frequencia de 200hz, e o ponto desligado
// - Se as duas chaves estiverem pressionadas, o ponto deve piscar a uma determinada frequencia, diferente da anterior

//Rafael V. Volkmer - 4324 / N°21 | Prova - Segundo Trimestre (2)
#include "stm32f4xx.h"

//PSC
#define FrequenciaPSC (16000-1)
///ARR (220ms)
#define Frequencia220 (1000-1)


//ARR PONTO
#define FrequenciaARR_PONTO (333-1)
#define FrequenciaARR_PONTO_SW1 (142-1)

#define mascara 0b1100000

uint8_t SegundoTempo;
uint8_t PrimeiroTempo;

uint8_t i;

uint16_t teclas;

uint8_t ponto = 0b10000000;

const uint8_t vet[8]=
{

	0b00000000, //null
        0b00000001, // 0
        0b00000011, // 1
        0b00000111, // 2
        0b00001111, // 3
        0b00011111, // 4
        0b00111111, // 5

};

void Contador ()
{

	GPIOC->ODR &=~ 0b1111111;
	GPIOC->ODR |= vet[i];

	PrimeiroTempo = TIM11->SR & TIM_SR_UIF;

	if(PrimeiroTempo)
	{
		TIM11->SR &=~ TIM_SR_UIF;
		
		i++;
	
		 if(i>7)
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
	TIM11->ARR = Frequencia220;

	TIM11->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;

	//configurações TIMER10
	TIM10->PSC = FrequenciaPSC;

	TIM10->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;

while (1)
{

	 teclas = GPIOA->IDR & mascara;
	 teclas = teclas >> 5;

	 switch(teclas)
	 {
	          case 0:
	        	Contador ();
	        	GPIOC->ODR &=~ ponto;
	        		break;

	          case 1:
	          	Contador ();
	          	PiscaPonto ();
	          	TIM10->ARR = FrequenciaARR_PONTO;
	          		break;

	          case 2:
	          	Contador ();
	          	GPIOC->ODR &=~ ponto;
	          		break;

	           case 3:
	          	Contador ();
	          	PiscaPonto ();
	          	TIM10->ARR = FrequenciaARR_PONTO_SW1;
	              	  	 break;
		 
	}
 }
}
