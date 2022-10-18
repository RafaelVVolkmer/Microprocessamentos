#ifndef COMANDOS_H_
#define COMANDOS_H_

#define Nenhum ' '

typedef enum {Entra_Senha = 0, Compara, Senha_Certa, Senha_Errada, Fecha_Armario} opera_t;

opera_t Operacao = Entra_Senha;

typedef enum{Agora,Antes,Pressionada} instante_t;

char Tecla [3];

uint16_t Armario_Escolhido = 0;

int i = 0;

char Senha_Digitada[6];

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

//FUNÇÃO DE DETECÇÃO DE BORDA PARA O TECLADO MATRICIAL
void TeclaPressionada ()
{
	Tecla[Antes] = Tecla[Agora];

	Tecla[Agora] = VarreduraTeclado();

	if(Tecla[Agora] != Tecla[Antes])
	{

		Tecla[Pressionada] = Tecla[Agora];

	}
	else
	{

		Tecla[Pressionada] = Nenhuma_Tecla;

	}

}
//FUNÇÃO QUE MUTIPLEXA OS LEDS NO ODR - FREQUÊNCIA DA RETINA - SEMPRE ATIVA
void MatrizLeds ()
{

	static int contador = -1;

	if(TIM11->SR & TIM_SR_UIF)
	{

		GPIOC->ODR = Leds [contador];

		TIM11->SR &=~ TIM_SR_UIF;

		contador++;

		if(contador > 9)
		{
			contador = 0;
		}

	}
}

//FUNÇÃO DE INÍCIO - ENTRADA DE DADOS - INSERIR SENHA
void InserirSenha ()
{

	  GPIOA->ODR = GPIO_ODR_ODR_0 | GPIO_ODR_ODR_1;

		  if(Tecla[Pressionada] != Nenhuma_Tecla && Tecla[Pressionada] != 'P' && Tecla[Pressionada] != 'E')
		  {

			  Senha_Digitada [i] = Tecla[Pressionada];

			  i++;

		  }

		  if(Tecla[Pressionada] == 'P')
		  {

			  Senha_Digitada [i] = ' ';

			  i--;


			  if(i == -2)
			  {
				  i = -1;
			  }
		  }

	  if(i >= 6)
	  {

		  Operacao = Compara;
	  }

}

//FUNÇÃO QUE COMPARA AS SENHA - RETORNA A POSIÇÃO DA SENHA CORRESPONDENTE EM RELAÇÃO À MATRIZ DE LEDS
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

//FUNÇÃO QUE EXECUTA A COMPARAÇÃO E LEVA PARA O PRÓXIMO ESTADO - SENHA CERTA OU SENHA ERRADA
void Comparacao ()
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

//FUNÇÃO STANDART PARA, DEPOIS DE 1 SEGUNDO DOS LEDS DE CONTROLE ACESSOS, TROCAR OPERAÇÃO
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

//FUNÇÃO QUE INSERE O VALOR DOS LEDS ATIVOS, DE ACORDO COM A POSIÇÃO DE COMPARAÇÃO, NO VETOR DE AMOSTRAGEM
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

//FUNÇÃO QUE SINALIZA O ERRO DA SENHA
void ErrouSenha ()
{

	GPIOA->ODR &=~ GPIO_ODR_ODR_0;

	RetornaIncio ();

}

//FUNÇÃO QUE INICIA A CHAMADA DO DESLIGAMENTO DOS LED APÓS PRESSIONAR A TECLA E - SEMPRE ATIVA
void ChamaFechadura ()
{

		if(Tecla[Pressionada] == 'E')
		{
			i = 0;

			Operacao = Fecha_Armario;

		}

}

// FUNÇÃO QUE INSERE O VALOR DE DESLIGAMENTO DOS LEDS NO VETOR DE AMOSTRAGEM EM RELAÇÃO À TECLA PRESSIONADA
void FecharArmario ()
{

	switch(Tecla[Pressionada])
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

		if(Tecla[Pressionada] == 'P')
		{

			i = 0;

			Operacao = Entra_Senha;

		}

}

#endif /* COMANDOS_H_ */
