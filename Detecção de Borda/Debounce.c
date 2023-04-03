/* USER CODE BEGIN PFP */

uint8_t bounce = 1;

/* USER CODE END PFP */

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  //Prevent unused argument(s) compilation warning
	UNUSED(GPIO_Pin);

	  if(GPIO_Pin == BUTTON_Pin && bounce == 1)
	  {

		  HAL_TIM_Base_Start_IT(&htim1); 

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

	  if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_RESET)
	  {

		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

		  bounce = 1;

		  HAL_TIM_Base_Stop_IT(&htim1);

	  }

}

/* USER CODE END 4 */
