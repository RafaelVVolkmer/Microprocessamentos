/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//DEBOUNCE
volatile uint8_t button_state[2] = {GPIO_PIN_SET, GPIO_PIN_SET};
volatile uint8_t last_button_state[2] = {GPIO_PIN_SET, GPIO_PIN_SET};
static uint8_t first_press = 0;
static uint8_t Click = 0; //toggle when you press


static unsigned int button_press = 0;

//TIM DIVISION
static uint8_t timDiv_counter_1 = 0;
static uint8_t timDiv_counter_3 = 0;

static int freq[4] =
{
		ONE_SEC, TWO_SEC, THREE_SEC, 0
};

static const uint16_t DisplayNumber[10]=
{

    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

static const uint32_t DisplayLetter[26]=
{

    0b01110111, //A
    0b01111100, //B
    0b00111001, //C
    0b01011110, //D
    0b01111001, //E
    0b01110001, //F
    0b00111101, //G
    0b01110100, //H
    0b00110000, //I
    0b00011110, //J
    0b01110101, //K
    0b00111000, //L
    0b00010101, //M
    0b00110111, //N
    0b00111111, //O
    0b01110011, //P
    0b01100111, //Q
    0b00110011, //R
    0b01101101, //S
    0b01111000, //T
    0b00111110, //U
    0b00011100, //V
    0b00101010, //W
    0b00110110, //X
    0b01101110, //Y
    0b01011011  //Z
};

static const uint8_t DisplayPins[7] =
{

		A_Pin,
		B_Pin,
		C_Pin,
		D_Pin,
		E_Pin,
		F_Pin,
		G_Pin

};

//COUNTERS
static unsigned int number_ofClicks = 0;
static int digit = -1;

void RESTART ()
{

	HAL_TIM_Base_Stop_IT(&htim11);

	number_ofClicks = 0;

	timDiv_counter_3 = 0;

	first_press = 0;

	Click = 0;

}

void Counter_Click (const unsigned int max)
{

	UNUSED(max);

	if(Click)
	{

		Click = 0;

		digit++;

		if (digit > max)
		{
			digit = 0;
		}

	}
}

void Counter_NumberOfClicks (const unsigned int seconds)
{

	UNUSED(seconds);

  	if(timDiv_counter_3 == seconds)
  	{

  		switch(number_ofClicks)
  		{


  			case 1:

  				HAL_TIM_Base_Start_IT(&htim10);

  				__HAL_TIM_SET_AUTORELOAD(&htim10, ONE_SEC);

  				RESTART();

  				break;

  			case 2:

  				HAL_TIM_Base_Start_IT(&htim10);

  			  	__HAL_TIM_SET_AUTORELOAD(&htim10, TWO_SEC);

  			  	RESTART();

  			  	break;

  			case 3:

  				HAL_TIM_Base_Start_IT(&htim10);

  			  	__HAL_TIM_SET_AUTORELOAD(&htim10, THREE_SEC);

  			  	RESTART();

  			  	break;

  			case 4:

  				HAL_GPIO_WritePin(PONTO_GPIO_Port, PONTO_Pin, GPIO_PIN_SET);

  				HAL_TIM_Base_Stop_IT(&htim10);

  				RESTART();

  				break;

  			default:

  				RESTART();

  				break;

  		}

  	}

}

void Counter_SecsPressed (const unsigned int seconds)
{

	UNUSED(seconds);

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

		__HAL_TIM_SET_AUTORELOAD(&htim10, TWO_SEC);

		button_press = 0;

		RESTART();


	}

}

void DisableAll()
{

	for(int i = 0; i <= 9; i++)
	{

		HAL_GPIO_WritePin(A_GPIO_Port, DisplayPins[i], GPIO_PIN_RESET);

	}

}
void write_digit(uint8_t digit)
{

	UNUSED(digit);

	for(int i = 0; i <= 9; i++)
	{

		HAL_GPIO_WritePin(A_GPIO_Port, DisplayPins[i], (DisplayNumber[digit] >> i) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);

	}

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM11_Init();
  MX_TIM1_Init();
  MX_TIM10_Init();

  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);

  DisableAll();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	 write_digit(digit);

	 Counter_Click(9);

	 if(first_press)
	  {

		 HAL_TIM_Base_Start_IT(&htim11);

		 Counter_NumberOfClicks (4);

	 }

	 //Counter_SecsPressed (4);


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	UNUSED(htim);

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

            	first_press = 1;

            	number_ofClicks ++;

            }

        }

        if (button_state[1] != last_button_state[1])
        {

            if (button_state[1] == GPIO_PIN_RESET)
            {

            	Click = 1;

            }

        }

    }

    if (htim->Instance == TIM10)
    {

      	HAL_GPIO_TogglePin(PONTO_GPIO_Port, PONTO_Pin);


    }

    if (htim->Instance == TIM11)
    {

    	timDiv_counter_3 ++;

    	timDiv_counter_1++;

    }

    else
    {

    	__NOP();

    }
}



/* USER C */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
