//Rafael V. Volkmer - 4324 / N°21 | Teclado Musical
#include "stm32f4xx.h"

#define FrequenciaPSC (1000-1)
#define FrequenciaARR (160-1)
#define mascara 0b01
#define para 0b10000000

#define do 261,63
#define DOs 279
#define re 293,66
#define REs 311,13
#define mi 329,63
#define fa 349,23
#define FAs 369,99
#define sol 392
#define sols 415,30
#define la 440
#define LAs 466,16
#define si 493,88

#define DO
#define RE

uint8_t tempo;
uint8_t teclas;
unsigned int i;

const unsigned float CruelAngelThesis[224] =
{
  //Rem, fa, sol, fa, sol, sol, sol ,do ,la# ,la ,sol ,la - la ,do ,re ,sol ,fa ,do ,do ,la ,do ,do , re, -
  //fa ,dom ,dom ,fa ,fa ,sol ,dom ,dom - dom ,la ,la# ,la ,sol ,fa ,sol ,la ,la# ,la ,rem, -
  //mi ,mi ,fa ,fa ,mi ,mi ,fa ,sol ,la# ,la ,sol ,fa ,la ,la ,sol ,fa ,sol ,rem ,rem, mi, mi, -
  //fa ,dom ,dom ,fa ,fa ,sol ,dom ,dom - dom ,la ,la# ,la ,sol ,fa ,sol ,la ,la# ,la ,rem, -
  //mi ,mi ,fa ,fa ,mi ,mi ,fa ,sol ,la# ,la ,sol ,fa ,la ,la ,sol ,fa ,sol ,la, la#, la, -
  //fa, fa, mi, fa, fa, mi, sol, sol, fa, mi, re, mi, fa, fa, mi, sol, mi, rem, dom, -
  //fa, fa, mi, fa, fa, mi, sol, sol, fa, mi, fa, sol, la, la#, la, sol, fa, sol, la, -
  //Rem, fa, sol, fa, sol, sol, sol ,do ,la# ,la ,sol ,la - la ,do ,re ,sol ,fa, mi, mi, re, mi, mi, mi, fa, -
  //Rem, fa, sol, fa, sol, sol, sol ,do ,la# ,la ,sol ,la - la ,do ,re ,sol ,fa ,do ,do ,la ,do ,do ,re, -
  //Rem, fa, sol, fa, sol, sol, sol ,do ,la# ,la ,sol ,la - la ,do ,re ,sol ,fa ,do ,do ,la ,do ,do ,re, -
};

void indice()
{
  tempo = TIM10->SR & TIM_SR_UIF;
  i = 0;
  
  if(tempo==0)
  {
    i++
  }
  
}

//Função para ativar saída com o botão pressionado
void chaveamento()
{
  
	teclas = GPIOC->IDR & mascara;
	tempo = TIM10->SR & TIM_SR_UIF;
	
	if(teclas)
	{
    
		if(tempo)
		{
			TIM10->SR =~ TIM_SR_UIF;
			
			GPIOA->ODR ^= GPIO_ODR_ODR_7;
		}
    
	}
  
}

int main(void)
{

 	//Ativando clock GPIOA e TIMER10
  	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
  	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
  
  	//Botão - PB0
 	GPIOB->MODER &=~ GPIO_MODER_MODER0;
  	GPIOB->MODER |= GPIO_MODER_MODER0_1;
  
  	//Botão - PB0 é Pull down
  	GPIOB->PUPDR &=~ GPIO_PUPDR_PUPDR0;
  	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR0_0;
  
	// PINO PA7 - SAÍDA DO SOM
	GPIOA->MODER &=~ GPIO_MODER_MODER7;
	GPIOA->MODER |= GPIO_MODER_MODER7_0;

	//Definindo frequências para o TIMER10
	TIM10->PSC = FrequenciaPSC;
	TIM10->ARR = FrequenciaARR;

	//Habilitando contagem do TIMER10
	TIM10->CR1 = TIM_CR1_CEN|TIM_CR1_ARPE;

	while (1)
	{
     	 	teclas = GPIOB->IDR & mascara;
      		tempo = TIM10->SR & TIM_SR_UIF;
    
      		chaveamento()
        
      			if(teclas==0)
      			{ 
          			GPIOA->ODR &=~ para;
      			} 
       				else
        			{   
            				indice()
            
            				TIM10->ARR = CruelAngelThesis[i];
           				TIM10->SR &=~ TIM_SR_UIF;
         			}
      }
    
}
