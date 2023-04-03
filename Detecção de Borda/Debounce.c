/* USER CODE BEGIN PFP */

uint8_t bounce = 1;

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
			
			//MAKE WHAT YOU WANT TO DO HERE, THE PREVIOUS LINE IS A EXAMPLE HOW TO TOGGLE A LED PIN. ERASE. EXCLUDE IF YOU WANT TO DO SOMETHING ELSE
		
			bounce = 1;
		
			HAL_TIM_Base_Stop_IT(&htim1); //Pass the same TIMER as the other function

		}
	}
	
}

/* USER CODE END 4 */
