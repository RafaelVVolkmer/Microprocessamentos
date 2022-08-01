//Rafael.V.Volkmer (4324); N° 21 - 24/05/2022. Matricula = 20000213

#include "stm32f4xx.h"

#define mascara 0b11110000000

uint16_t contador;

const uint16_t leds[16]=
{
		0b0000000, // 0000 | 0  | led = void
		0b0010000, // 0001 | 1  | led = 1
		0b1010000, // 0010 | 2  | led = 1 3
		0b0000100, // 0011 | 3  | led = 0
		0b0010000, // 0100 | 4  | led = 1
		0b0000100, // 0101 | 5  | led = 0
		0b0000100, // 0110 | 6  | led = 0
		0b0010000, // 0111 | 7  | led = 1
		0b0010000, // 1000 | 8  | led = 1
		0b0100100, // 1001 | 9  | led = 0 2
		0b0000100, // 1010 | 10 | led = 0
		0b0010000, // 1011 | 11 | led = 1
		0b0000100, // 1100 | 12 | led = 0
		0b0010000, // 1101 | 13 | led = 1
		0b0010000, // 1110 | 14 | led = 1
		0b0000100  // 1111 | 15 | led = 0
};



int main(void)
{

  //Habilita clock dos registradores B e C
  RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

  //Zerando os pinos de C
  GPIOC->MODER&=~(GPIO_MODER_MODER2 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6);
  //Saídas PC2 PC4 PC5 PC6
  GPIOC->MODER|=GPIO_MODER_MODER2_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0;

  //Entradas PB10 PB9 PB8 PB7
  GPIOB->MODER&=~(GPIO_MODER_MODER10 | GPIO_MODER_MODER9 | GPIO_MODER_MODER8 | GPIO_MODER_MODER7);

  //Zerando PULL-DOWNS
  GPIOB->PUPDR&=~(GPIO_PUPDR_PUPDR10 | GPIO_PUPDR_PUPDR9 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR7);
  //PB10 PB9 PB8 PB7 são PULL-DOWNS
  GPIOB->PUPDR|=GPIO_PUPDR_PUPDR10_1 | GPIO_PUPDR_PUPDR9_1 | GPIO_PUPDR_PUPDR8_1 | GPIO_PUPDR_PUPDR7_1;

    while (1)
   {
      contador=GPIOB->IDR & mascara;
      contador=contador >> 7;

      GPIOC->ODR=leds[contador];

   }

}
