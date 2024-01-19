
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
#include <string.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef struct
{

  float period_segundos;
  uint32_t periodo;

  float frequencia;

  float defasagem;
  float momento;

  uint32_t borda_subida[2];
  uint32_t borda_descida[2];

  float duty_cycle;
  float largura_pulso;
  float largura_pulso_segundos;

} SignalData;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MSGSIZE 1000
#define TMSG 100

#define TCMD 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

enum estado {atual, anterior};

SignalData signalData1;
SignalData signalData2;

static uint8_t msg[MSGSIZE] = {[0...MSGSIZE], 0};
static uint8_t comando[TCMD] = {[0...TCMD], 0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM2_Init();
  MX_TIM10_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim10);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); //LIGA O TIM 5 COMO INPUT CAPTURE EM INTERRUPT
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2); //LIGA O TIM 5 COMO INPUT CAPTURE EM INTERRUPT
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3); //LIGA O TIM 5 COMO INPUT CAPTURE EM INTERRUPT
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4); //LIGA O TIM 5 COMO INPUT CAPTURE EM INTERRUPT

  HAL_TIM_OC_Start(&htim5, TIM_CHANNEL_1); //LIGA GERAÇÃO DE SINAL
  HAL_TIM_OC_Start(&htim5, TIM_CHANNEL_2); //LIGA GERAÇÃO DE SINAL

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (__HAL_TIM_GET_FLAG(&htim10, TIM_FLAG_UPDATE))
	  {
	       __HAL_TIM_CLEAR_FLAG(&htim10, TIM_FLAG_UPDATE);

	       snprintf(msg, MSGSIZE,
	    		   "\r\n\n----------PRIMEIRO SINAL----------\r\n\n"

	    		   "Periodo: %.2f s\r\n"
	    		   "Frequencia: %.2f Hz\r\n"
	    		   "Tempo em Alto: %.2f s\r\n"
	    		   "Duty Cycle: %.2f / 100\r\n\n"

	    		   "Borda de Subida (atual): %ld\r\n"
	    		   "Borda de Subida (anterior): %ld\r\n"

	    		   "Borda de Descida (atual): %ld\r\n"
	    		   "Borda de Descida (anterior): %ld\r\n"

				   "\r\n\n----------SEGUNDO SINAL-----------\r\n\n"

	    		   "Periodo: %.2f s\r\n"
	    		   "Frequencia: %.2f Hz\r\n"
	    		   "Tempo em Alto: %.2f s\r\n"
	    		   "Duty Cycle: %.2f / 100\r\n\n"

	    		   "Borda de Subida (atual): %ld\r\n"
	    		   "Borda de Subida (anterior): %ld\r\n"

	    		   "Borda de Descida (atual): %ld\r\n"
	    		   "Borda de Descida (anterior): %ld\r\n"

				   "-----------------------------------\r\n\n"
	    		   "Defasagem (primeiro pro segundo): %.2f graus // momento: %.4f",

				   signalData1.period_segundos,
				   signalData1.frequencia,
				   signalData1.largura_pulso_segundos,
				   signalData1.duty_cycle,
				   signalData1.borda_subida[atual],
				   signalData1.borda_subida[anterior],
				   signalData1.borda_descida[atual],
				   signalData1.borda_descida[anterior],

				   signalData2.period_segundos,
				   signalData2.frequencia,
				   signalData2.largura_pulso_segundos,
				   signalData2.duty_cycle,
				   signalData2.borda_subida[atual],
				   signalData2.borda_subida[anterior],
				   signalData2.borda_descida[atual],
				   signalData2.borda_descida[anterior],

				   signalData1.defasagem,
				   signalData1.momento
				   );

	       HAL_UART_Transmit(&huart2, msg, MSGSIZE, TMSG);
	   }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	UNUSED(huart);

	comando[0]=toupper(comando[0]);

	//Implementar lógica caso necessário uso de serial - Por padrão, não precisa
	switch(comando[0])
	{

		case 'E':


				break;

		case 'D':

				break;

		case 'P':

				break;

		case 'R':

				break;

		default:

			strcpy(msg,"\r\nParametro invalido\r\n");

				break;
	}

 	HAL_UART_Receive_IT(&huart2, comando,TCMD);

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	UNUSED(htim);

	if(htim->Instance == TIM2)
	{

		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{

			  signalData1.borda_subida[anterior] = signalData1.borda_subida[atual];

			  signalData1.borda_subida[atual] = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1);

			  signalData1.periodo = signalData1.borda_subida[atual] - signalData1.borda_subida[anterior];

			  signalData1.period_segundos = (float)signalData1.periodo * 0.000001f;

			  signalData1.frequencia = 1 / signalData1.period_segundos;

		}

		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{

			 signalData2.borda_subida[anterior] = signalData2.borda_subida[atual];

			 signalData2.borda_subida[atual] = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_2);

			 signalData2.periodo = signalData2.borda_subida[atual] - signalData2.borda_subida[anterior];

			 signalData2.period_segundos = (float)signalData2.periodo * 0.000001f;

			 signalData2.frequencia = 1 / signalData2.period_segundos;

		 }

		 if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		 {

			signalData1.borda_descida[anterior] = signalData1.borda_descida[atual];

			signalData1.borda_descida[atual] = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_4);

			signalData1.largura_pulso = signalData1.borda_descida[atual] - signalData1.borda_subida[atual];

			signalData1.largura_pulso_segundos = (float)signalData1.largura_pulso * 0.000001f;

			signalData1.duty_cycle = ((float)signalData1.largura_pulso / signalData1.periodo) * 100.0f;

		 }

		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{

			signalData2.borda_descida[anterior] = signalData2.borda_descida[atual];

			signalData2.borda_descida[atual] = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_3);

			signalData2.largura_pulso = signalData2.borda_descida[atual] - signalData2.borda_subida[atual];

			signalData2.largura_pulso_segundos = (float)signalData2.largura_pulso * 0.000001f;

			signalData2.duty_cycle = ((float)signalData2.largura_pulso / signalData2.periodo) * 100.0f;


		}
		else
		{
			__NOP();
		}


		 signalData1.momento = (float)(signalData2.borda_subida[atual] - signalData1.borda_subida[atual]) * 0.000001f;

		 signalData1.defasagem = 360 * signalData1.momento / signalData1.period_segundos;

	}
	else
	{
		__NOP();
	}

}

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
