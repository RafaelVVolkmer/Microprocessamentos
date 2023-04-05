#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

void SystemClock_Config(void);

//SEIRAL DEIFINES
#define TAM_MSG 50
#define TCMD 1 
#define TMSG 200
#define TOUT 1000

//SERIAL VARIABLES
char comand[TCMD];
char msg[200] = "\n\rCOMANDO SERIAL\r\n";

//DEBOUNCE VARIABLES
volatile uint8_t button_state[2] = {GPIO_PIN_SET, GPIO_PIN_SET};
volatile uint8_t last_button_state[2] = {GPIO_PIN_SET, GPIO_PIN_SET};

//COUNTERS
uint8_t click = 0;
int digit = -1;
uint8_t stop = 0;

//STRINGS
uint8_t DisplayNumber[6]=
{

    	0b00000001, // A_Pin activated
	0b00000010, // B_Pin activated
	0b00000100, // C_Pin activated
	0b00001000, // D_Pin activated
	0b00010000, // E_Pin activated
	0b00100000, // F_Pin activated

};

const uint8_t DisplayPins[7] =
{

	A_Pin, //PC0
	B_Pin, //PC1
	C_Pin, //PC2
	D_Pin, //PC3
	E_Pin, //PC4
	F_Pin, //PC5
	G_Pin  //PC6

};

//FUNCTION TO DETECTE CLICKS
void Counter_NumberOfClicks (const unsigned int Click)
{

	UNUSED(Click);

  		switch(Click)
  		{

  			case 1:

				//CHANGE THE FREQUENCE
  				__HAL_TIM_SET_AUTORELOAD(&htim10, one_ms);

  				HAL_UART_Transmit(&huart2, "\rfrequencia alterada para 100ms\r\n", 28, TOUT);

  				break;

  			case 2:

				//CHANGE THE FREQUENCE
  			  	__HAL_TIM_SET_AUTORELOAD(&htim10, two_ms);

  			  	HAL_UART_Transmit(&huart2, "\rfrequencia alterada para 200ms\r\n", 28, TOUT);

  			  	break;

  			case 3:
				
				//CHANGE THE FREQUENCE
  			  	__HAL_TIM_SET_AUTORELOAD(&htim10, five_ms);

  			  	HAL_UART_Transmit(&huart2, "\rfrequencia alterada para 500ms\r\n", 28, TOUT);

  			  	break;

  			case 4:

				//CHANGE THE FREQUENCE
  				__HAL_TIM_SET_AUTORELOAD(&htim10, one_second);

  				HAL_UART_Transmit(&huart2, "\rfrequencia alterada para um segundo\r\n", 28, TOUT);

  				break;

  		}
}

//FUNCTION TO RESET DISPLAY (ACTIVATE ON 0)
void DisableAll()
{
	
	for(int i = 0; i <= 6; i++)
	{

		HAL_GPIO_WritePin(A_GPIO_Port, DisplayPins[i], GPIO_PIN_SET);

	}

}

//FUNTION TO WRITE IN DISPLAY (ACTIVATE IN 0)
void write_digit(uint8_t digit)
{

	UNUSED(digit);

	for(int i = 0; i <= 6; i++)
	{

		HAL_GPIO_WritePin(A_GPIO_Port, DisplayPins[i], (DisplayNumber[digit] >> i) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);

	}

}

int main(void)
{
 
  	HAL_Init();

  	SystemClock_Config();

  	MX_GPIO_Init();
  	MX_USART2_UART_Init();
 	 MX_TIM1_Init();
 	 MX_TIM10_Init();
 	 MX_TIM11_Init();
 
  	HAL_TIM_Base_Start_IT(&htim1);
  	HAL_TIM_Base_Start_IT(&htim10);

  	HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);

 	 HAL_UART_Receive_IT(&huart2, comand, TCMD);

  	DisableAll();
  
 	 while (1)
 	 {

	 	 if (msg[0] != '\0')
	 	 {
		  
	     		HAL_UART_Transmit(&huart2, msg, strlen((char *) msg), 1000);
	     		msg[0] = '\0';
		  
	  	 }

  	}
  
}

//TIMER INTERRUPT
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	UNUSED(htim);

 	 //DEBOUNCE
    	if (htim->Instance == TIM1)
    	{

       		last_button_state[0] = button_state[0];

        	button_state[0] = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);

       		last_button_state[1] = button_state[1];

       		button_state[1] = HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin);


        	if (button_state[0] != last_button_state[0])
        	{

           		if (button_state[0] == GPIO_PIN_RESET)
            		{

            			click ++;

            			if(click >= 5)
            			{
					
            				click = 1;
            			}

            			Counter_NumberOfClicks (Click);

            		}

       		 }

      		if (button_state[1] != last_button_state[1])
        	{
		
            		if (button_state[1] == GPIO_PIN_RESET)
            		{

            			stop =! stop;

            			if(stop)
            			{
					
            				HAL_TIM_Base_Stop_IT(&htim10);
            			}
            			else
            			{
					
            				HAL_TIM_Base_Start_IT(&htim10);
				}

           		 }

     		}

 	}

  	//SEQUENCE OF DISPLAY
    	if (htim->Instance == TIM10)
    	{

    		write_digit(digit);

    		digit ++;

    		if (digit > 5)
    		{
			
    			digit = 0;
			
    		}	


   	 }
   	 else
    	{

    		__NOP();

   	 }
	
}

//USART INTERRUPT
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	UNUSED(huart);

	comand[0]=toupper(comand[0]);

  	//TRANSMIT
	switch(comand[0])
	{
		
		//THIS TWO CASES TURN THE ORDER OS THE SEQUENCE IN DUSPLAY
		case 'E':

			DisplayNumber[0] = 0b00000001;
			DisplayNumber[1] = 0b00000010;
			DisplayNumber[2] = 0b00000100;
			DisplayNumber[3] = 0b00001000;
			DisplayNumber[4] = 0b00010000;
			DisplayNumber[5] = 0b00100000;

			strcpy(msg,"\r\nEsquerda\r\n");

			HAL_GPIO_TogglePin(PONTO_GPIO_Port, PONTO_Pin);

			write_digit(digit);

				break;

		case 'D':

			DisplayNumber[0] = 0b00100000;
			DisplayNumber[1] = 0b00010000;
			DisplayNumber[2] = 0b00001000;
			DisplayNumber[3] = 0b00000100;
			DisplayNumber[4] = 0b00000010;
			DisplayNumber[5] = 0b00000001;

			strcpy(msg,"\r\nDireita\r\n");

			HAL_GPIO_TogglePin(PONTO_GPIO_Port, PONTO_Pin);

			write_digit(digit);

				break;
		
		//THIS CASE STOP THE SEQUENCE
		case 'P':

			HAL_TIM_Base_Stop_IT(&htim10);

			strcpy(msg,"\r\nParou\r\n");

			HAL_GPIO_TogglePin(PONTO_GPIO_Port, PONTO_Pin);

			write_digit(digit);

				break;
			
		//THIS CASE START THE SEQUENCE AGAIN
		case 'R':

			HAL_TIM_Base_Start_IT(&htim10);

			strcpy(msg,"\r\nVoltou\r\n");

			HAL_GPIO_TogglePin(PONTO_GPIO_Port, PONTO_Pin);

			write_digit(digit);

				break;

		default:

			strcpy(msg,"\r\nParametro invalido\r\n");

				break;
	}

 	HAL_UART_Receive_IT(&huart2, comand,TCMD);

}
