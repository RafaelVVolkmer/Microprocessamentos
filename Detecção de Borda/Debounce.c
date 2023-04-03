/* USER CODE BEGIN PFP */

//DEBOUNCE
uint8_t bounce = 1;
uint8_t first_press = 0;
uint8_t Click = 0; //toggle when you press

//TIM DIVISION
uint8_t timDiv_counter_1 = 0, timDiv_counter_3 = 0;

//COUNTERS
unsigned int button_press = 0;
unsigned int number_ofClicks = 0;

/* USER CODE END PFP */

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  	//Prevent unused argument(s) compilation warning
	UNUSED(GPIO_Pin);

	//Pass the name of your BUTTON INTERRUPT PIN, that you should define in "LABEL" as X_Pin (in this case, i named "BUTTON")
	if(GPIO_Pin == BUTTON_Pin && bounce) 
	{

		HAL_TIM_Base_Start_IT(&htim1); // Pass the TIMER you want as a parameter (in this case, I opted for TIM1)

		bounce = 0;

	}
	else
	{

		__NOP();

	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	//Prevent unused argument(s) compilation warning
	UNUSED(htim);
	
	//Checks if the TIMER INSTANCE set for the debounce frequency was the one who called the interrupt (same TIMER as the other function)
	if(htim->Instance == TIM1)
	{
		if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_SET)
		{
		
			//Pass the name of your OUTPUT PIN and PORT, that you should define in "LABEL" as X_GPIO_Port and X_Pin (in this case, i named "LED")
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin); 
			
			//MAKE WHAT YOU WANT TO DO WITH THESE VARIABLES THE PREVIOUS LINE IS A EXAMPLE HOW TO TOGGLE A LED PIN. EXCLUDE IF YOU WANT TO DO SOMETHING ELSE
		
			bounce = 1; //used to reset the debounce
			
			first_press = 1; //used to target the first time your pressed the button

  			number_ofClicks ++; //increment your number of click

  			Click = 1; //activte things with just one click
			
			HAL_TIM_Base_Stop_IT(&htim1); //Pass the same TIMER as the other function

		}
	}
}

/* USER CODE END 4 */

//BUTTON OPTIONS

/* USER CODE BEGIN 0 */

void TIM_Division_PerSecond ()
{
	if(__HAL_TIM_GET_FLAG(&htim11, TIM_FLAG_UPDATE))
	{

		 __HAL_TIM_CLEAR_FLAG(&htim11, TIM_FLAG_UPDATE);

		 timDiv_counter_1 ++;

		 timDiv_counter_3 ++;

	}
}

void RESTART ()
{

	HAL_TIM_Base_Stop(&htim11);

	number_ofClicks = 0;

	timDiv_counter_3 = 0;

	first_press = 0;

}

void Counter_SecsPressed (const unsigned int seconds)
{
	if(timDiv_counter_1)
	{

		timDiv_counter_1 = 0;

		if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin))
		{

			button_press ++;

		}
		else
		{

			button_press = 0;

		}

	}

	if(button_press == seconds)
	{

		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

		 button_press = 0;

		  RESTART();

	}
}

void Counter_NumberOfClicks (const unsigned int seconds)
{
  	if(timDiv_counter_3 == seconds)
  	{
		
  		switch(number_ofClicks)
  		{

  			case 1:

  				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

  				RESTART();

  				break;

  			default:

  				RESTART();

  				break;

  		}
		
  	}
}

/* USER CODE END 0 */

int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM11_Init();
  MX_TIM1_Init();

  while (1)
  {
	  
	  TIM_Division_PerSecond ( );

	  Counter_SecsPressed (3);

	  if(first_press)
	  {

	  	HAL_TIM_Base_Start(&htim11);

	  	Counter_NumberOfClicks (3);

	 }

  }
	
}
