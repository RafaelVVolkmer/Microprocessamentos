//Rafael V. Volkmer - 4324 / N°21 | Teclado Musical
#include "stm32f4xx.h"

#define FrequenciaPSC (1-1)
#define FrequenciaARR (1-1)

#define FrequenciaPSC_2 (1000-1)
#define FrequenciaARR_2 (160-1)

#define mascara 0b01
#define para 0b10000000

#define Do 261
#define DOs 279.
#define Re 293
#define REs 311
#define Mi 329
#define Fa 349
#define FAs 369
#define Sol 392
#define Sols 415
#define La 440
#define LAs 466
#define Si 493

#define DO
#define RE

uint8_t tempo;
uint8_t teclas;
unsigned int i;


const unsigned int CruelAngelThesis[224] =
{
    	RE, Fa, Sol, Fa, Sol, Sol, Sol, Do, LAs, La, Sol, La, 0, La, Do, Re, Sol, Fa, Do, Do, La, Do, Do, Re, 0,
	Fa, DO, DO, Fa, Fa, Sol, DO, DO, 0, DO, La, LAS, La, Sol, Fa, Sol, La, LAS, La, RE, 0,
	Mi, Mi, Fa, Fa, Mi, Mi, Fa, Sol, LAs, La, Sol, Fa, La, La, Sol, Fa, Sol, RE, RE, Mi, Mi, 0,
	Fa, DO, DO, Fa, Fa, Sol, DO, DO, 0, DO, La, LAs, La, Sol, Fa, Sol, La, LAs, La, RE, 0,
	Mi, Mi, Fa, Fa, Mi, Mi, Fa, Sol, La, Las, La, Sol, Fa, La, La, Sol, Fa, Sol, La, LAs, La, 0,
	Fa, Fa, Mi, Fa, Fa, Mi, Sol, Sol, Fa, Mi, Re, Mi, Fa, Fa, Mi, Sol, Mi, RE, DO, 0,
	Fa, Fa, Mi, Fa, Fa, Mi, Sol, Sol, Fa, Mi, Fa, Sol, La, LAs, La, Sol, Fa, Sol, La, 0,
	RE, Fa, Sol, Fa, Sol, Sol, Sol, Do, LAs, La, Sol, La, 0, La, Do, Re, Sol, Fa, Do, Do, La, Do, Do, Re, 0,
	RE, Fa, Sol, Fa, Sol, Sol, Sol, Do, LAs, La, Sol, La, 0, La, Do, Re, Sol, Fa, Do, Do, La, Do, Do, Re
	RE, Fa, Sol, Fa, Sol, Sol, Sol, Do, LAs, La, Sol, La, 0, La, Do, Re, Sol, Fa, Mi, Mi, Re, Mi, Mi, Mi, Fa  
};

void indice()
{
	
  	tempo = TIM11->SR & TIM_SR_UIF;
  	i = 0;
  
 	 if(tempo==1)
  	{
    		i++;
		TIM11->SR &=~ TIM_SR_UIF;
		
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
  	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN | RCC_APB2ENR_TIM11EN;
  
  	//Botão - PB0
 	GPIOB->MODER &=~ GPIO_MODER_MODER0;
  
  	//Botão - PB0 é Pull down
  	GPIOB->PUPDR &=~ GPIO_PUPDR_PUPDR0;
  	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR0_1;
  
	// PINO PA7 - SAÍDA DO SOM
	GPIOA->MODER &=~ GPIO_MODER_MODER7;
	GPIOA->MODER |= GPIO_MODER_MODER7_0;

	//Definindo frequências para os timers
	TIM10->PSC = FrequenciaPSC;
	TIM10->ARR = FrequenciaARR;
	
	TIM11->PSC = FrequenciaPSC_2
	TIM11->PSC = FrequenciaARR
		
	//Habilitando contagem para os timer
	TIM10->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
	
	
	while (1)
	{
     	 	teclas = GPIOB->IDR & mascara;
      		tempo = TIM10->SR & TIM_SR_UIF;
			
      		chaveamento()
		
        
      			if(teclas == 0)
      			{ 
          			GPIOA->ODR &=~ para;
      			} 
       				else
        			{   
            				indice()
						
						while (i < 245)
						{
							TIM11->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
            
            						TIM10->ARR = CruelAngelThesis[i];
           						TIM10->SR &=~ TIM_SR_UIF;
						}
         			}
      }
}
