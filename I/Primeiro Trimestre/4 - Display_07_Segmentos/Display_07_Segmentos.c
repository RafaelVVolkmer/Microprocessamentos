//Rafael.V.Volkmer (4324); Nº21 - 06/05/2022.

#include "stm32f4xx.h"

#define mascara 0b01111

uint16_t contador;

const uint16_t DisplayHexa[16]=
 {
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
     0b01110001  // F
 };


int main(void)
{

	// Habilita clock dos registradores B e C
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

	//Zerando os pinos B
	GPIOB->MODER&=~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6);
	//Saídas - PB0, PB1, PB2, PB3, PB4, PB5, PB6
	GPIOB->MODER|=GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0;

	//Entradas - PC0, PC1, PC2, PC3
	GPIOC->MODER&=~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3);

	// Zerando os os pinos C
	GPIOC->PUPDR&=~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3);
	//Entradas - PC0, PC1, PC2, PC3 são PULL DOWN
	GPIOC->PUPDR|=GPIO_PUPDR_PUPDR0_1 | GPIO_PUPDR_PUPDR1_1 | GPIO_PUPDR_PUPDR2_1 | GPIO_PUPDR_PUPDR3_1;


  while (1)
  {

	 contador=GPIOC->IDR & mascara; //Mascara IDR com as entradas PC0, PC1, PC2, PC3
	 
		//Caso for catodo, mantenha esta linha:
		GPIOB->ODR=DisplayHexa[contador];
		
		//Caso for anodo, comente a linha anterior e descomente esta linha:
    		//GPIOB->ODR=~DisplayHexa[Contador];
  }
}
