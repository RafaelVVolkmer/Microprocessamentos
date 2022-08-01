//Rafael.V.Volkmer (4324); N° 21 - 06/05/2022.

#include "stm32f4xx.h"

#define mascara 0b011

uint8_t contador;

const uint8_t BinarioLed[4]=
{
      0b000,   // 0
      0b001,   // 1
      0b011,   // 2
      0b111,   // 3
};

int main(void)
{

  //Habilita clock dos registradores B e C

  RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

  //Zerando os pinos de C
  GPIOC->MODER&=~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2);
  //Saídas PC0 PC1 e PC2
  GPIOC->MODER|=GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 |  GPIO_MODER_MODER2_0;

  //Entradas PB0 e PB1
  GPIOB->MODER&=~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1);

  //Zerando PULL-DOWNS
  GPIOB->PUPDR&=~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1);
  //PB0 e PB1 são PULL-DOWNS
  GPIOB->PUPDR|=GPIO_PUPDR_PUPDR0_1 | GPIO_PUPDR_PUPDR1_1;

    while (1)
   {
      contador=GPIOB->IDR & mascara;

      GPIOC->ODR=BinarioLed[contador];

  }
}
