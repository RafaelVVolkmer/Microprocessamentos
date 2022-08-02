//Rafael.V.Volkmer (4324); Nº21 - 10/05/2022.

#include "stm32f4xx.h"

//Defines para letras aparecerem nos Displays
#define n 0b00110111
#define s 0b01101101
#define e 0b01111001
#define o 0b00111111

#define mascara 0b11110000

#define apaga 0b00

//Variável para bifshift de controle do DisplayC2
#define DisplayC2 7

//     0   1   2  3   4  5  6  7   9  10 11 12 13 14 15  16
enum {N=0,NNE,NNO,NO,ENE,NE,E,ESE,OSO,SO,O,ONO,S,SSO,SSE,SE};

uint8_t contador;

int main(void)
{

		// Habilita clock dos registradores B, C e A
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

		//Zerando os pinos C
		GPIOC->MODER&=~
			(
			GPIO_MODER_MODER0 | 
			GPIO_MODER_MODER1 | 
			GPIO_MODER_MODER2 | 
			GPIO_MODER_MODER3 | 
			GPIO_MODER_MODER4 | 
			GPIO_MODER_MODER5 | 
			GPIO_MODER_MODER6
			);
	
		//Saídas - PC0, PC1, PC2, PC3, PC4, PC5, PC6
		GPIOC->MODER|=
			GPIO_MODER_MODER0_0 | 
			GPIO_MODER_MODER1_0 | 
			GPIO_MODER_MODER2_0 | 
			GPIO_MODER_MODER3_0 | 
			GPIO_MODER_MODER4_0 | 
			GPIO_MODER_MODER5_0 | 
			GPIO_MODER_MODER6_0;

		//Zerando os pinos C*2
		GPIOC->MODER&=~
			(
			GPIO_MODER_MODER7  |
			GPIO_MODER_MODER8  | 
			GPIO_MODER_MODER9  | 
			GPIO_MODER_MODER10 | 
			GPIO_MODER_MODER11 | 
			GPIO_MODER_MODER12 | 
			GPIO_MODER_MODER13
			);
	
		//Saídas - PC7, PC8, PC9, PC10, PC11, PC12, PC13
		GPIOC->MODER|=
			GPIO_MODER_MODER7_0  | 
			GPIO_MODER_MODER8_0  | 
			GPIO_MODER_MODER9_0  | 
			GPIO_MODER_MODER10_0 | 
			GPIO_MODER_MODER11_0 | 
			GPIO_MODER_MODER12_0 | 
			GPIO_MODER_MODER13_0;

		//Zerando os pinos B
		GPIOB->MODER&=~
			(
			GPIO_MODER_MODER0 |
			GPIO_MODER_MODER1 | 
			GPIO_MODER_MODER2 | 
			GPIO_MODER_MODER3 | 
			GPIO_MODER_MODER4 | 
			GPIO_MODER_MODER5 | 
			GPIO_MODER_MODER6
			);
	
		//Saídas - PB0, PB1, PB2, PB3, PB4, PB5, PB6
		GPIOB->MODER|=
			GPIO_MODER_MODER0_0 | 
			GPIO_MODER_MODER1_0 | 
			GPIO_MODER_MODER2_0 | 
			GPIO_MODER_MODER3_0 | 
			GPIO_MODER_MODER4_0 | 
			GPIO_MODER_MODER5_0 | 
			GPIO_MODER_MODER6_0;

		//Zerando os pinos A | Entradas PA4, PA5, PA6, PA7
		GPIOA->MODER&=~(GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7);

		// Zerando os os pinos PULL DOWN
		GPIOA->PUPDR&=~(GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);
		//Entradas - PA4, PA5, PA6, PA7 são PULL DOWN
		GPIOA->PUPDR|=GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1 | GPIO_PUPDR_PUPDR6_1 | GPIO_PUPDR_PUPDR7_1;

  	  while (1)
  {

  		 contador=GPIOA->IDR & mascara; //Mascara IDR com as entradas PA4, PA5, PA6, PA7
  		 contador=contador >>4; //Faz com que PA4, PA5, PA6 e PA7 tornem-se PA0, PA1, PA2 e PA3 para parâmetros de contagem.

  		  	  GPIOB->ODR=apaga;
  		  	  GPIOC->ODR=apaga;

   	   switch(contador)
   	   {
   	   	   	   	   case N:
   	   	   	   		   GPIOB->ODR=n;
   	   	   	   		   	   break;
   	   	   	   	   case NNE:
   	   	   	   		   GPIOB->ODR=n;
   	   	   	   		   GPIOC->ODR=n | (e<<DisplayC2);
   	   	   	   		   	   break;
   	   	   	   	   case NNO:
   	   	   	   		   GPIOB->ODR=n;
   	   	   	   		   GPIOC->ODR=n | (o<<DisplayC2);
   	   	   	   		   	   break;
   	   	   	   	   case NO:
   	   	   	   		   GPIOB->ODR=n;
   	   	   	   		   GPIOC->ODR=o;
   	   	   	   		   	   break;
   	   	   	   	   case ENE:
   	   	   	   		   GPIOB->ODR=e;
   	   	   	   		   GPIOC->ODR=n | (e<<DisplayC2);
   	   	   	   		   	   break;
   	   	   	   	   case NE:
   	   	   	   		   GPIOB->ODR=n;
   	   	   	   		   GPIOC->ODR=e;
   	   	   	   		   	   break;
   	   	   	   	   case E:
   	   	   	   		   GPIOB->ODR=e;
   	   	   	   		   	   break;
   	   	   	   	   case ESE:
   	   	   	   		   GPIOB->ODR=e;
   	   	   	   		   GPIOC->ODR=s | (e<<DisplayC2);
   	   	   	   		   	   break;
   	   	   	   	   case OSO:
   	   	   	   		   GPIOB->ODR=o;
   	   	   	   		   GPIOC->ODR=s | (o<<DisplayC2);
   	   	   	   		   	   break;
   	   	   	   	   case SO:
   	   	   	   		   GPIOB->ODR=s;
   	   	   	   		   GPIOC->ODR=o;
   	   	   	   		   	   break;
   	   	   	   	   case O:
   	   	   	   		   GPIOB->ODR=o;
   	   	   	   		   	   break;
   	   	   	   	   case ONO:
   	   	   	   		   GPIOB->ODR=o;
   	   	   	   		   GPIOC->ODR=n | (o<<DisplayC2);
   	   	   	   		   	   break;
   	   	   	   	   case S:
   	   	   	   		   GPIOB->ODR=s;
   	   	   	   		   	   break;
   	   	   	   	   case SSO:
   	   	   	   		   GPIOB->ODR=s;
   	   	   	   		   GPIOC->ODR=s | (o<<DisplayC2);
   	   	   	   		   	   break;
   	   	   	   	   case SSE:
   	   	   	   		   GPIOB->ODR=s;
   	   	   	   		   GPIOC->ODR=s | (e<<DisplayC2);
   	   	   	   		   	   break;
   	   	   	   	   case SE:
   	   	   	   		   GPIOB->ODR=s;
   	   	   	   		   GPIOC->ODR=e;
   	   	   	   		   	   break;
   	   	   	   	  default:
   	   	   	   		  GPIOB->ODR=apaga;
				  	  GPIOC->ODR=apaga;
				  	  	 	break;
   	   	  }
  	  }
}

