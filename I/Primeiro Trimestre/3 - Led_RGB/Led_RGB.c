//Rafael.V.Volkmer (4324); Nº21. - 06/05/2022.

#include "stm32f4xx.h"

#define mascara 0b0111

uint8_t contador;

const uint8_t CorDoLed[8]=
		{
		  	0b0000, //Apagado   - Void
			0b0001, //Azul      - Pc0
			0b0010, //Verde     - Pc1
			0b0011, //Ciano     - Pc0, Pc1
			0b0100, //Vermelho  - Pc2
			0b0101, //Magenta   - Pc0, Pc2
			0b0110, //Amarelo   - Pc1, Pc2
			0b0111  //Branco    - Pc0, Pc1, Pc2
		};

int main(void)
{

	// Habilita clock dos registradores B e C
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

	//Zerando os pinos B
	GPIOB->MODER&=~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2);
	//Saídas - PB0, PB1, PB2
	GPIOB->MODER|=GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0;

	//Zerando os pinos C | Entradas - PC0, PC1, PC2
	GPIOC->MODER&=~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2);

	// Zerando os os pinos PULL-DOWN
	GPIOC->PUPDR&=~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2);
	//Entradas - PC0, PC1, PC2 são PULL DOWN
	GPIOC->PUPDR|=GPIO_PUPDR_PUPDR0_1 | GPIO_PUPDR_PUPDR1_1 | GPIO_PUPDR_PUPDR2_1;



		while (1)
  	  {

		contador=GPIOC->IDR & mascara; //Mascara IDR com as entradas PC0, PC1, PC2

		GPIOB->ODR=CorDoLed[contador];

  	  }
}


