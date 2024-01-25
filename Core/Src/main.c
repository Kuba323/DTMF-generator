/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "dac.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include <stdbool.h>
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
#define PI 3.1415926
GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
uint32_t previousMillis = 0;
uint32_t currentMillis = 0;


volatile bool irq_flag = false;
volatile uint32_t i=0, l=0;
volatile uint16_t buffer1[64];
uint16_t sin_amp=0x0fff;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	for(uint8_t j=0; j<64; j++){
//		buffer1[j]=((sin_amp+1)/2)*sin((2*M_PI*j)/64+1)+(sin_amp+1)/2;
	buffer1[j]=((sin(j*2*PI/64)+1)*(4096/2));
	}
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
  MX_DAC1_Init();
  MX_TIM7_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
  HAL_TIM_Base_Start_IT(&htim16);
  HAL_TIM_Base_Start_IT(&htim7);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{


	  if(htim->Instance==TIM16){
//		  if(irq_flag){
			  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, buffer1[l]);
			  l++;
			  if(l>=64) l=0;
	//		  HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
//		  }
	  }
	  if(htim->Instance==TIM7){
//		  if(irq_flag){
			  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, buffer1[i]);
			  i++;
			  if(i>=64) i=0;
//		  }
	  }
//	  irq_flag=false;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  irq_flag = true;
  currentMillis = HAL_GetTick();
  if (currentMillis - previousMillis > 10) {

    GPIO_InitStructPrivate.Pin = GPIO_PIN_3|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
    GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructPrivate);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
    {

    	TIM16->ARR = 1792;
    	__NOP();
    	TIM7->ARR = 1033;
    	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,1);
    }
    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
    {

    	TIM16->ARR = 1792;
    	__NOP();
    	TIM7->ARR = 936;
    	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,1);
    }
    else if(GPIO_Pin == GPIO_PIN_8 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8))
    {

    	TIM16->ARR = 1792;
    	__NOP();
    	TIM7->ARR = 845;
    	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,1);
    }
    else if(GPIO_Pin == GPIO_PIN_9 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9))
    {

    	TIM16->ARR = 1792;
    	__NOP();
    	TIM7->ARR = 764;
    	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,1);
    }
    else{
    	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
    {

    	TIM16->ARR = 1623;
    	__NOP();
    	TIM7->ARR = 1034;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
    {

    	TIM16->ARR = 1623;
    	__NOP();
    	TIM7->ARR = 937;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
    else if(GPIO_Pin == GPIO_PIN_8 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8))
    {

    	TIM16->ARR = 1623;
    	__NOP();
    	TIM7->ARR = 846;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
    else if(GPIO_Pin == GPIO_PIN_9 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9))
    {

    	TIM16->ARR = 1623;
    	__NOP();
    	TIM7->ARR = 765;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
    {

//    	__NOP();= 48; //ASCII value of 0
    	TIM16->ARR = 1467;
    	__NOP();
    	TIM7->ARR = 1034;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
    {

//    	TIM7->ARR = 938;
//    	__NOP();
    	TIM16->ARR =1467;
    	__NOP();
    	TIM7->ARR = 937;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
    else if(GPIO_Pin == GPIO_PIN_8 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8))
    {

//    	TIM7->ARR = 938;
//    	__NOP();
    	TIM16->ARR =1467;
    	__NOP();
    	TIM7->ARR = 846;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
    else if(GPIO_Pin == GPIO_PIN_9 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9))
    {

//    	TIM7->ARR = 938;
//    	__NOP();
    	TIM16->ARR =1466;
    	__NOP();
    	TIM7->ARR = 765;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
    if(GPIO_Pin == GPIO_PIN_3 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))
    {

//    	TIM7->ARR = 849;
//    	__NOP();
    	TIM16->ARR =1328;
    	__NOP();
    	TIM7->ARR = 1034;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
    {

//    	TIM7->ARR = 849;
//    	__NOP();
    	TIM16->ARR =1328;
    	__NOP();
    	TIM7->ARR = 937;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
    else if(GPIO_Pin == GPIO_PIN_8 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8))
    {

//    	TIM7->ARR = 849;
//    	__NOP();
    	TIM16->ARR =1328;
    	__NOP();
    	TIM7->ARR = 846;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }
    else if(GPIO_Pin == GPIO_PIN_9 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9))
    {

//    	TIM7->ARR = 849;
//    	__NOP();
    	TIM16->ARR =1328;
    	__NOP();
    	TIM7->ARR = 765;
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);

    GPIO_InitStructPrivate.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructPrivate.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructPrivate);
    previousMillis = currentMillis;
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
