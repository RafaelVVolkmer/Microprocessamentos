#ifndef TECLADOMATRICIAL_H_
#define TECLADOMATRICIAL_H_

#include "stm32f4xx.h"

#define Pinos_Colunas 0b1110000
#define Pinos_Linhas 0b1111

#define Primeira_Coluna 0b11100000
#define Segunda_Coluna 0b11010000
#define Terceira_Coluna 0b10110000

#define Primeira_Linha 0b1110
#define Segunda_Linha 0b1101
#define Terceira_Linha 0b1011
#define Quarta_Linha 0b0111

#define Nenhuma_Tecla '\0'

char VarreduraTeclado ()
{

	char tecla = Nenhuma_Tecla;

	static uint16_t i = 0;

	GPIOB->ODR |= Pinos_Colunas;

	GPIOB->ODR &= Primeira_Coluna;

	i = GPIOB-> IDR & Pinos_Linhas;

	if(i != Pinos_Linhas)
	{

		switch(i)
		{

		case Primeira_Linha:

			tecla = '1';

				break;

		case Segunda_Linha:

			tecla = '4';

				break;

		case Terceira_Linha:

			tecla = '7';

				break;

		case Quarta_Linha:

			tecla = 'P';

				break;

		}

	}

	GPIOB->ODR |= Pinos_Colunas;

	GPIOB->ODR &= Segunda_Coluna;

	i = GPIOB->IDR & Pinos_Linhas;

	if(i != Pinos_Linhas)
	{

		switch(i)
		{

		case Primeira_Linha:

			tecla = '2';

				break;

		case Segunda_Linha:

			tecla = '5';

				break;

		case Terceira_Linha:

			tecla = '8';

				break;

		case Quarta_Linha:

			tecla = '0';

				break;

		}

	}

	GPIOB->ODR |= Pinos_Colunas;

	GPIOB->ODR &= Terceira_Coluna;

	i = GPIOB->IDR & Pinos_Linhas;

	if(i != Pinos_Linhas)
	{

		switch(i)
		{

			case Primeira_Linha:

				tecla = '3';

					break;

			case Segunda_Linha:
				tecla = '6';

					break;

			case Terceira_Linha:

				tecla = '9';

					break;

			case Quarta_Linha:

				tecla = 'E';

					break;

		}
	}

	return tecla;

}


#endif /* TECLADOMATRICIAL_H_ */
