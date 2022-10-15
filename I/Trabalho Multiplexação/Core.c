//Trabalho Multiplexação - Segundo Trimestre - MICROS I.  21/09/2022
//Feito Por: Rafael V. Volkmer (Nº21) e Pedro Trento (Nº20)

#include "stm32f4xx.h"
#include "TecladoMatricial.h"

#define Nenhum ' '

#define Frequencia_ARR (1000-1)
#define Frequencia_PSC (16000-1)

typedef enum {Entra_Senha = 0, Senha_Certa, Senha_Errada} opera_t;

opera_t Operacao = Entra_Senha;

char Senha_Digitada[6];

uint16_t Armario_Escolhido = 0;

char Tecla = Nenhuma_Tecla;

//Função que compara as senhas
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

	uint8_t Coluna = 0, Posicao = 0, Comparacao = 0;
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

//Função que recebe a senha
void InserirSenha ()
{

	  static uint8_t Borda = 0;

	  static uint8_t i = 0;

	  GPIOA->ODR |= GPIO_ODR_ODR_1 | GPIO_ODR_ODR_0;

	  if(Borda == 0)
	  {

		  if(Tecla != Nenhuma_Tecla)
		  {

			  Senha_Digitada [i] = Tecla;

			  i++;

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

//Função quem abre o armario selecionado
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

	GPIOA->ODR &=~ GPIO_ODR_ODR_1;

	GPIOC->ODR |= Armarios [Armario_Escolhido];

		if(TIM10->SR & TIM_SR_UIF)
		{
			TIM10->SR &=~ TIM_SR_UIF;

			GPIOA->ODR |= GPIO_ODR_ODR_1;

			Operacao = Entra_Senha;

		}

}

//Função que sinaliza o erro de senha
void ErrouSenha ()
{

	GPIOA->ODR &=~ GPIO_ODR_ODR_0;

}

int main(void)
{

	//Habilitando Clocks
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

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

  while (1)
  {

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

	    }

  }
}
