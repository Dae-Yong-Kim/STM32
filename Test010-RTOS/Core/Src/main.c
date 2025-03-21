/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "C:\Users\user\source\repos\STM32\myLib\Inc\uSonic.h"
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
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

osThreadId myTask01Handle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTask04Handle;
osSemaphoreId myBinarySem01Handle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
void myStartTask01(void const * argument);
void myStartTask02(void const * argument);
void myStartTask03(void const * argument);
void myStartTask04(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int mode = 0, bn = 0;
double dist = 1000;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//printf("B1 button pressed..... %d times\r\n", bn++);
	/*bn++;
	mode = 1;*/

	//osSemaphoreRelease(myBinarySem01Handle);
}

void LD2Test() {
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of myBinarySem01 */
  osSemaphoreDef(myBinarySem01);
  myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of myTask01 */
  osThreadDef(myTask01, myStartTask01, osPriorityNormal, 0, 128);
  myTask01Handle = osThreadCreate(osThread(myTask01), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, myStartTask02, osPriorityBelowNormal, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, myStartTask03, osPriorityLow, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* definition and creation of myTask04 */
  osThreadDef(myTask04, myStartTask04, osPriorityIdle, 0, 512);
  myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  ProgramStart("RTOS test.........");
  HAL_TIM_Base_Start(&htim2);
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  LD2Test();
	  //HAL_Delay(500);	Don't use HAL_Delay (RTOS)
	  osDelay(500);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

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
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 84-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, In4_Pin|In1_Pin|In3_Pin|In2_Pin
                          |Trig_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Echo_Pin */
  GPIO_InitStruct.Pin = Echo_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Echo_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : In4_Pin In1_Pin In3_Pin In2_Pin
                           Trig_Pin */
  GPIO_InitStruct.Pin = In4_Pin|In1_Pin|In3_Pin|In2_Pin
                          |Trig_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_myStartTask01 */
/**
  * @brief  Function implementing the myTask01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_myStartTask01 */
void myStartTask01(void const * argument)
{
  /* USER CODE BEGIN 5 */
	//int n1 = 0;
  /* Infinite loop */
  for(;;)
  {
	/*LD2Test();
	//HAL_Delay(500);
	Cursor(0, 0);
	printf("LD2 flashed %d times(Task1)\r\n", n1++);
    osDelay(500);*/
	if(osSemaphoreWait(myBinarySem01Handle, 0) == osOK) { // running time is lower than 80ms
		dist = Distance();
		//printf("Task01 (Normal)=====\r\n");
		osSemaphoreRelease(myBinarySem01Handle);
	}
	osDelay(100);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_myStartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_myStartTask02 */
void myStartTask02(void const * argument)
{
  /* USER CODE BEGIN myStartTask02 */
	//int n2 = 0;
  /* Infinite loop */
  for(;;)
  {
	//Cursor(0, 10);
	/*printf("Input Degree : ");
	scanf("%d", &n2);
	int step = 4096 * n2 / 360;
	printf("Wave(Full) : %d steps, Half : %d steps", step / 2, step);*/

	/*if(mode) {
		printf("B1 button pressed..... %d times", bn);
		if(++t1 > 10) {
			mode = 0;
			t1 = 0;
		}
		else {
			printf("                              ");
		}
	}*/

	/*if(mode) {
		mode = 0;
		for(int i = 0; i < 64 * bn; i++) {
			HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 1);
			HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 0);
			HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 0);
			HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 0);
			osDelay(10);


			HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 1);
			HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 1);
			HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 0);
			HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 0);
			osDelay(10);

			HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 0);
			HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 1);
			HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 0);
			HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 0);
			osDelay(10);

			HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 0);
			HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 1);
			HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 1);
			HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 0);
			osDelay(10);

			HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 0);
			HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 0);
			HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 1);
			HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 0);
			osDelay(10);

			HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 0);
			HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 0);
			HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 1);
			HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 1);
			osDelay(10);

			HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 0);
			HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 0);
			HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 0);
			HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 1);
			osDelay(10);

			HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 1);
			HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 0);
			HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 0);
			HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 1);
			osDelay(10);
		}
		bn = 0;
	}
	else {
		HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 0);
		HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 0);
		HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 0);
		HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 0);
		osDelay(10);
	}
    osDelay(1);*/



	if(osSemaphoreWait(myBinarySem01Handle, 0) == osOK) {
		//printf("Task02 (BelowNormal)----\r\n");

		if(dist < 150.0) {
			int degree = (dist > 75.0) ? 1 : 2;
			for(int i = 0; i < 64 * degree; i++) {
				HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 1);
				HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 0);
				HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 0);
				HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 0);
				HAL_Delay(5);

				HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 0);
				HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 1);
				HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 0);
				HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 0);
				HAL_Delay(5);

				HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 0);
				HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 0);
				HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 1);
				HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 0);
				HAL_Delay(5);

				HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 0);
				HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 0);
				HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 0);
				HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 1);
				HAL_Delay(5);
			}
			bn = 0;
		}
		else {
			HAL_GPIO_WritePin(In1_GPIO_Port, In1_Pin, 0);
			HAL_GPIO_WritePin(In2_GPIO_Port, In2_Pin, 0);
			HAL_GPIO_WritePin(In3_GPIO_Port, In3_Pin, 0);
			HAL_GPIO_WritePin(In4_GPIO_Port, In4_Pin, 0);
			HAL_Delay(5);
		}
		osSemaphoreRelease(myBinarySem01Handle);
	}
	osDelay(100);
  }
  /* USER CODE END myStartTask02 */
}

/* USER CODE BEGIN Header_myStartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_myStartTask03 */
void myStartTask03(void const * argument)
{
  /* USER CODE BEGIN myStartTask03 */
  /* Infinite loop */
  for(;;)
  {
	  if(osSemaphoreWait(myBinarySem01Handle, 0) == osOK) {
		  //printf("Task03 (Low)****\r\n");
		  LD2Test();
		  osSemaphoreRelease(myBinarySem01Handle);
	  }
	  osDelay(100);
  }
  /* USER CODE END myStartTask03 */
}

/* USER CODE BEGIN Header_myStartTask04 */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_myStartTask04 */
void myStartTask04(void const * argument)
{
  /* USER CODE BEGIN myStartTask04 */
  /* Infinite loop */
  for(;;)
  {
	if(osSemaphoreWait(myBinarySem01Handle, 0) == osOK) {
		//printf("Task04 (Idle)+++\r\n");
		Cursor(0, 10);
		printf("distance: %6.2fmm                   \r\n", dist);
		osSemaphoreRelease(myBinarySem01Handle);
	}
	osDelay(100);
  }
  /* USER CODE END myStartTask04 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM10 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM10) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
