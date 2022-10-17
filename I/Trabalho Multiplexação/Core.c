//Trabalho Multiplexação - Segundo Trimestre - MICROS I.  21/09/2022
//Feito Por: Rafael V. Volkmer (Nº21) e Pedro Trento (Nº20)

#include "stm32f4xx.h"
#include "TecladoMatricial.h"

#define Nenhum ' '

#define Tecla_Apagar 'P'

#define Frequencia_ARR (500-1)
#define Frequencia_PSC (16000-1)

typedef enum {Entra_Senha = 0, Senha_Certa, Senha_Errada, Fecha_Armario} opera_t;

opera_t Operacao = Entra_Senha;

char Senha_Digitada[6];

uint8_t i = 0;

uint16_t e = 0;

uint8_t Borda = 0;

uint16_t Armario_Escolhido = 0;

char Tecla = Nenhuma_Tecla;

uint16_t Leds [9] =
{
		0b0,
		0b0,
		0b0,
		0b0,
		0b0,
		0b0,
		0b0,
		0b0,
		0b0
};

void MatrizLeds ()
{

	if(TIM11->SR & TIM_SR_UIF)
	{

		GPIOC->ODR = Leds [e];

		TIM11->SR &=~ TIM_SR_UIF;

		e++;

		if(e > 9)
		{
			e = 0;
		}

	}
}

uint16_t Comparador ()
{

	const char Senhas [9][6] =
	{
			{'1', '1', '1', '1', '1', '1'},
			{'2', '2', '2', '2', '2', '2'},
			{'3', '3', '3', '3', '3', '3'},
			{'4', '4', '4', '4', '4', '4'},
			{'5', '5', '5', '5', '5', '5'},
			{'6', '6', '6', '6', '6', '6'},
			{'7', '7', '7', '7', '7', '7'},
			{'8', '8', '8', '8', '8', '8'},
			{'9', '9', '9', '9', '9', '9'}
	};

	uint8_t Coluna= 0, Posicao = 0, Comparacao = 0;
	uint16_t Linha = 0;

	while(Comparacao != 6 && Linha < 9)
	{

		if(Senhas[Linha] [Coluna] == Senha_Digitada[Posicao])
		{

			Comparacao ++;

		}

		Coluna ++;

		Posicao ++;

		if((Coluna && Posicao == 6) && (Comparacao != 6))
		{

			Linha ++;

			Comparacao = 0;
			Coluna = 0;
			Posicao = 0;

		}
	}

	if(Comparacao != 6)
	{
		Linha = Nenhum;
	}

	return Linha;
}

void RetornaIncio ()
{

	if(TIM10->SR & TIM_SR_UIF)
	{
		TIM10->SR &=~ TIM_SR_UIF;

	}
	TIM10->CNT = 0;

	while((TIM10->SR & TIM_SR_UIF) == 0)
	{

		i = 0;

		TIM10->SR &=~ TIM_SR_UIF;

		Operacao = Entra_Senha;

		MatrizLeds ();
	}

}

void InserirSenha ()
{

	  GPIOA->ODR = GPIO_ODR_ODR_0 | GPIO_ODR_ODR_1;

	  if(Borda == 0)
	  {

		  if(Tecla != Nenhuma_Tecla && Tecla != 'P' && Tecla != 'E')
		  {

			  Senha_Digitada [i] = Tecla;

			  i++;

			  Borda ++;

		  }

		  if(Tecla == Tecla_Apagar)
		  {
			  Senha_Digitada [i] = ' ';

			  i--;

			  Borda ++;
		  }


	  }
	  else
	  {

		  if(Tecla == Nenhuma_Tecla)
		  {
			  Borda = 0;
		  }

	  }

	  if(i >= 6)
	  {

		  Armario_Escolhido = Comparador();

		  if(Armario_Escolhido == Nenhum)
		  {

			  Operacao = Senha_Errada;

		  }
		  else
		  {

			  Operacao = Senha_Certa;

		  }

	}

}

void AbrirArmario ()
{
	uint16_t Armarios [9] =
	{
			  0b001001,
			  0b010001,
			  0b100001,
			  0b001010,
			  0b010010,
			  0b100010,
			  0b001100,
			  0b010100,
			  0b100100
	};

	Leds[Armario_Escolhido] = Armarios[Armario_Escolhido];

	GPIOA->ODR &=~ GPIO_ODR_ODR_1;

	RetornaIncio ();

}

void ErrouSenha ()
{

	GPIOA->ODR &=~ GPIO_ODR_ODR_0;

	RetornaIncio ();

}

void ChamaFechadura ()
{

	if(Borda == 0)
	{
		if(Tecla == 'E')
		{
			i = 0;

			Operacao = Fecha_Armario;

			Borda++;
		}
	}
	else
	{

		if(Tecla == Nenhuma_Tecla)
		{
			Borda = 0;
		}

	}

}

void FecharArmario ()
{

	switch(Tecla)
	{

		case '1':

			Leds [0] = 0b0;

				break;

		case '2':

			Leds [1] = 0b0;

				break;

		case '3':

			Leds [2] = 0b0;

				break;

		case '4':

			Leds [3] = 0b0;

				break;

		case '5':

			Leds [4] = 0b0;

				break;

		case '6':

			Leds [5] = 0b0;

				break;

		case '7':

			Leds [6] = 0b0;

				break;

		case '8':

			Leds [7] = 0b0;

				break;

		case '9':

			Leds [8] = 0b0;

				break;
	}

	if(Borda == 0)
	{
		if(Tecla == 'P')
		{

			i = 0;

			Operacao = Entra_Senha;

			Borda++;

		}
	}
	else
	{

		if(Tecla == Nenhuma_Tecla)
		{

			Borda = 0;

		}

	}

}

int main(void)
{

	//Habilitando Clocks
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN | RCC_APB2ENR_TIM11EN;

	//Entradas
		GPIOB->MODER &=~
				(
						GPIO_MODER_MODER0 |
						GPIO_MODER_MODER1 |
						GPIO_MODER_MODER2 |
						GPIO_MODER_MODER3
				);

	//Entradas PULL- UP
		GPIOB->PUPDR &=~
				(
						GPIO_PUPDR_PUPDR0 |
						GPIO_PUPDR_PUPDR1 |
						GPIO_PUPDR_PUPDR2 |
						GPIO_PUPDR_PUPDR3
				);

		GPIOB->PUPDR |=
						GPIO_PUPDR_PUPDR0_0 |
						GPIO_PUPDR_PUPDR1_0 |
						GPIO_PUPDR_PUPDR2_0 |
						GPIO_PUPDR_PUPDR3_0;

	//Saídas Teclado
		GPIOB->MODER &=~
				(
						GPIO_MODER_MODER4 |
						GPIO_MODER_MODER5 |
						GPIO_MODER_MODER6 |
						GPIO_MODER_MODER7
				);

		GPIOB->MODER |=
						GPIO_MODER_MODER4_0 |
						GPIO_MODER_MODER5_0 |
						GPIO_MODER_MODER6_0 |
						GPIO_MODER_MODER7_0;

	//Saídas LEDS
		GPIOC->MODER &=~
				(
						GPIO_MODER_MODER0 |
						GPIO_MODER_MODER1 |
						GPIO_MODER_MODER2 |
						GPIO_MODER_MODER3 |
						GPIO_MODER_MODER4 |
						GPIO_MODER_MODER5
				);

		GPIOC->MODER |=
						GPIO_MODER_MODER0_0 |
						GPIO_MODER_MODER1_0 |
						GPIO_MODER_MODER2_0 |
						GPIO_MODER_MODER3_0 |
						GPIO_MODER_MODER4_0 |
						GPIO_MODER_MODER5_0;

	//Led de Controle de Estado
		GPIOA->MODER &=~ (GPIO_MODER_MODER0 | GPIO_MODER_MODER1);

		GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0;

	//Configuações do Timer
		TIM10->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;

		TIM10->ARR = Frequencia_ARR;
		TIM10->PSC = Frequencia_PSC;

		TIM11->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;

		TIM11->ARR = 999;
		TIM11->PSC = 15;

  while (1)
  {

	  MatrizLeds ();

	  ChamaFechadura();

	  Tecla = VarreduraTeclado();

	    switch (Operacao)
	    {

	    	case Entra_Senha:

	    		InserirSenha();

	    			break;

	    	case Senha_Certa:

	    	  	AbrirArmario();

	    			break;

	    	case Senha_Errada:

	    	  	ErrouSenha();

	    			break;

	    	case Fecha_Armario:

	    		FecharArmario();

	    			break;

	    }

  }
}
