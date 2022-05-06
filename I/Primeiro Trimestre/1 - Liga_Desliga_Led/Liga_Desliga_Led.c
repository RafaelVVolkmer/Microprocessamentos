//Rafael.V.Volkmer (4324); N°21 - 06/05/2022.

#include "stm32f4xx.h"

uint16_t contador;

const uint8_t Mascara = 0b01;

const uint8_t LigaOuDesliga[2]=
{
	0b00 //desliga
	0b01 //Liga
};

int main(void)
{

      // Habilita clock dos registradores B e C
      RCC->AHBE1NR|=RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
	    
      //Entrada PB0
      GPIOB->MODER&=~PIO_MODER_MODER0;
      
      //Zerando pinos C
      GPIOC->MODER&=~PIO_MODER_MODER0;
      //Saída PC0
      GPIOC->MODER|=PIO_MODER_MODER0_0;
      
      //Zerando PULL-DOWNS
      GPIOB->PUPDR&=~GPIO_PUPDR_PUPDR0;
      //PB0 é PULL-DOWN
      GPIOB->PUPDR|=GPIO_PUPDR_PUPDR0_1;


        while (1)
       {
          	contador=GPIOB->IDR & Mascara; //Mascara com PB0
		
		//Se quiser que o led ligue quando for apertado o botão.
		GPIOC->ODR=LigaOuDesliga[contador];
		
		//Se quiser que o led desligue quando for apertado o botão, descomente a linha abaixo e comente a anterior.
		//GPIOC->ODR=~LigaOuDesliga[contador];
			
       }
}
