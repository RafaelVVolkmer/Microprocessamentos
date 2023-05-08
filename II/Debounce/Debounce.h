/*
----------------------------------------------------------------------------------------------
  					DEBOUNCE.H
----------------------------------------------------------------------------------------------

				Por: Rafael V. Volkmer
				Data: 08/05/2023
       
==============================================================================================
       
    	Essa biblioteca foi feita com o propósito de maximizar a eficiência
     	da detecção de borda e debounce de multíplos botões através de struct e
     	interrupções de Timer.
       
     	Utilizando o CubeIDE, ajuste seu PSC para (8400-1), e seu ARR para (500-1),
    	isso te dará uma frequência de 50ms, que é o necessário para fazer o debounce.
	
	Caso seja no Atollic, as funções HAL não estarão disponíveis, mas só mude-as para outro
	jeito de ler os valores atuais das entradas, o pino e o port. Nele, ajuste o PSC para (16000-1) e o ARR
	para (50-1).
       
     	Você deve incluir essa biblioteca dentro da sua main, utilizando: #include "Debounce.h",
    	após arrastar o arquivo para dentro da pasta Scr de seu projeto ou criar uma header
    	com esse mesmo código.
     	
    	Dentro de sua main, declare uma estrutura como variável global, seguindo este modelo:
       
     	PinIn_t button =
     	{

    		.Port = BT_GPIO_Port, (BT é o nome que dei ao meu pino nas configs. Mude de acordo com o seu)

    		.Pin = BT_0_Pin, (BT é o nome que dei ao meu pino nas configs. Mude de acordo com o seu)

    		.Activate_Val = GPIO_PIN_SET, (É ativo em 1? GPIO_PIN_SET. É ativo em 0? GPIO_PIN_RESET)

    		.Button_State = {0},

    		.Last_Rising_Edge_State = 0,

    		.Last_Falling_Edge_State = 1,
	
	};
		
		Button é o nome que dei à minha estrutura. Para manipular suas variávies, usarei
		button.nomedavariavél. Caso queira utilizar outro botão, posso criar outra estrutura,
		mudando os parâmetros e seu nome, como, por exemplo button2.nomedavariável.
		
		As variáveis que serão utilizadas para ver quando o botão foi pressionado serão
		
		nomedaestrutura.Falling_Edge, para borda de descida
		
		nomedaestrutura.Rising_Edge, para a bord de subida
		
		Use lógicas com if, while ou qualquer outra coisas para fazer com que essas variaveis 
		façam o que devem fazer.
		
		Para chamar a funcão, use, dentro da interrupção do seu timer:
		
		Debounce(&button);
		
		Onde button é o nome da minha estrutura referente ao primeiro botão.
		
		Também há, no final, a função Zero, que zera tanto a borda de subida como a de descida
		de seu botão. Para usa-la, digite Zero(&button); onde button é o nome da sua estrutura onde quer zera-la.
		
		Exemplo de como eu chamo duas funções, cada uma para um botão, dentro da interrupção do TIM 10:
		
		void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
		{

			UNUSED(htim);

	 		//DEBOUNCE
	   		 if (htim->Instance == TIM10)
	   		 {

	    			Debounce(&button);

	    			Debounce(&button2);

	   		}
	   		else
	    		{

	    			__NOP();

	   	 	}

		}
		
		Agora, caso o primeiro botão for pressionado, ligue e desligue um led:
		
		if(button.Rising_Edge)
		{

			Zero(&button);


			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

	 	}
   
 */

#ifndef SRC_DEBOUNCE_H_
#define SRC_DEBOUNCE_H_

#include "stm32f4xx_hal.h"

typedef struct
{
    GPIO_TypeDef* Port;

    uint16_t Pin;

    const uint8_t Activate_Val;

    volatile uint8_t Button_State[2];

    volatile uint8_t Last_Rising_Edge_State;

    volatile uint8_t Last_Falling_Edge_State;

    volatile uint8_t Rising_Edge;

    volatile uint8_t Falling_Edge;

} PinIn_t;

void Debounce(PinIn_t* button)
{

	UNUSED(button);

		//Verifica o estado atual do botÃ£o
	    uint8_t current_state = HAL_GPIO_ReadPin(button->Port, button->Pin) == button->Activate_Val ? 1 : 0;

	    if ( current_state != button->Button_State[1] )
	    {

	        button->Button_State[1] = current_state;

	        if ( current_state == 0 )
	        {
	        	//Verifica se houve uma borda de descida
	            if ( button->Button_State[0] > button->Last_Rising_Edge_State )
	            {

	                button->Falling_Edge = 1;

	            }

	            button->Last_Falling_Edge_State = button->Button_State[0];

	        }

	        else
	        {

	        	//Verifica se houve uma borda de subida
	            if ( button->Button_State[0] < button->Last_Falling_Edge_State )
	            {

	                button->Rising_Edge = 1;

	            }

	            button->Last_Rising_Edge_State = button->Button_State[0];

	        }

	    }
	    else
	    {

	    	__NOP();

	    }

	    button->Button_State[0] = current_state;

}

void Zero (PinIn_t* button)
{
	
	button->Rising_Edge = 0;
	
	button->Falling_Edge = 0;
	
}

#endif /* SRC_DEBOUNCE_H_ */
