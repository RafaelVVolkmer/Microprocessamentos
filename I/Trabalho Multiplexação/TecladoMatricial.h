/*
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
				    BIBLIOTECA "TecladoMatricial.h"
------------------------------------------------------------------------------------------------------------
			    Feito por: Rafael V. Volkmer - ft. Pedro Trento
____________________________________________________________________________________________________________

	Essa biblioteca foi feita para a realização do trabalho de demultiplexação dado a mim e minha dupla,
Pedro Trento, na disciplinas de Micros I, da Fundação Escola Técnica Liberato Salzano Vieira da Cunha.
	Aqui, há funções para ler e captar valores das teclas pressionadas pelo teclado matricial, dados pela
variável "Teclas". Para poder mexer certo com o hardwer, os pinos devem ser configurados defidamente na main,
assim como suas conexões com os pinos do arm devem ser corretas. As entradas devem estar em PULL-UP e as
saídas em OPEN - DRAIN. Segue a configuração abaixo:

------------------------------------------------------------------------------------------------------------
					    TECLADO MATRICIAL
------------------------------------------------------------------------------------------------------------

                                           C0    C1    C2    C3
          	  	  	  	  [PB4] [PB5] [PB6] [PB7]
			       L0  [PB0] |  1  |  2  |  3  |  A  |
			       L1  [PB1] |  4  |  5  |  6  |  B  |
			       L2  [PB2] |  7  |  8  |  9  |  C  |
			       L3  [PB3] |  *  |  0  |  #  |  D  |

------------------------------------------------------------------------------------------------------------
	  *Seguir o esquema acima é de suma importância para o bom funcionamento do código*
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

#include "stm32f4xx.h"

#define Timer TIM10->SR  //Aqui, pode mudar o timer que deseja usar (precisa ser compatível com o da sua main)

#define Numero_Colunas 4  //Aqui, pode mudar o número de colunas que tem em seu teclado

#define Pinos_Colunas 0b11110000  //Aqui, pode mudar os pinos das colunas
#define Pinos_Linhas 0b00001111   //Aqui, pode mudar os pinos das Linas

#define Nenhuma_Tecla '\0'

uint16_t Leitura[Numero_Colunas];
uint8_t coluna = 0;
uint16_t Tecla;

void VarreduraColunas ()
{

	const uint16_t Varredura[Numero_Colunas] =
		{
				0b11101111,
				0b11011111,
				0b10111111,
				0b01111111
		};

	   TIM10->SR &=~ TIM_SR_UIF;

	   	   GPIOB->ODR |= Pinos_Colunas;

	   	   GPIOB->ODR &= Varredura[coluna];

	   	   Leitura[coluna] = GPIOB->IDR | Pinos_Linhas;

}


void IdentificaTeclas ()
{

	uint16_t ColunaMatriz;

	const uint16_t Teclado_Matricial [4] [4] =
		{
				{'1','2','3','A'},
				{'4','5','6','B'},
				{'7','8','9','C'},
				{'*','0','#','D'}
		};


	if (++coluna == Numero_Colunas)
	   	   {

	   		   coluna = 0;

	   		   	   for(ColunaMatriz=0 ; ColunaMatriz < Numero_Colunas ; ColunaMatriz++)
	   		   	   {

	   		   		   	  switch(Leitura[ColunaMatriz])
	   		   		   	  {
                                                                         // C0  C1  C2  C3
	   		   		   	  	  case 0b1110 : // [1] [2] [3] [A] - L0
	   		   		   	  		  	  Tecla = Teclado_Matricial [0] [ColunaMatriz];
	   		   		   	  		  	  	  break;

	   		   		   	  		  	 // C0  C1  C2  C3
	   		   		   	  	  case 0b1101 : // [4] [5] [6] [B] - L1
	   		   		   	  		  	  Tecla = Teclado_Matricial [1] [ColunaMatriz];
	   		   		   	  		  	  	  break;

	   		   		   	  		         // C0  C1  C2  C3
	   		   		   	  	  case 0b1011 : // [7] [8] [9] [C] - L2
	   		   		   	  		  	  Tecla = Teclado_Matricial [2] [ColunaMatriz];
	   		   		   	  		  	  	  break;

	   		   		   	  		         // C0  C1  C2  C3
	   		   		   	  	  case 0b0111 : // [*] [0] [#] [D] - L3
	   		   		   	  		  	 Tecla = Teclado_Matricial [3] [ColunaMatriz];
	   		   		   	  		  	 	 break;

	   		   		   	  }

	   		   	   }

	   	   }
}

void SemTeclaAtiva ()
{
	if (Tecla != Nenhuma_Tecla)
	   	   {
	   		   Tecla = Nenhuma_Tecla;
	   	   }
}

void TecladoMatricial_Completo ()
{
	uint8_t tempo = Timer & TIM_SR_UIF;

	   if (tempo)
	   {

		   VarreduraColunas ();

		   IdentificaTeclas ();

	   }

	   	   SemTeclaAtiva ();

}
