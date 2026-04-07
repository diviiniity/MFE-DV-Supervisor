/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "gpio.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for monitorEBSTask */
osThreadId_t monitorEBSTaskHandle;
const osThreadAttr_t monitorEBSTask_attributes = {
  .name = "monitorEBSTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for monitorWDTask */
osThreadId_t monitorWDTaskHandle;
const osThreadAttr_t monitorWDTask_attributes = {
  .name = "monitorWDTask",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};
/* Definitions for transmitCANTask */
osThreadId_t transmitCANTaskHandle;
const osThreadAttr_t transmitCANTask_attributes = {
  .name = "transmitCANTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for controlACTTask */
osThreadId_t controlACTTaskHandle;
const osThreadAttr_t controlACTTask_attributes = {
  .name = "controlACTTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void monitorEBS(void *argument);
void monitorWD(void *argument);
void transmitCAN(void *argument);
void controlACT(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

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
  /* creation of monitorEBSTask */
  monitorEBSTaskHandle = osThreadNew(monitorEBS, NULL, &monitorEBSTask_attributes);

  /* creation of monitorWDTask */
  monitorWDTaskHandle = osThreadNew(monitorWD, NULL, &monitorWDTask_attributes);

  /* creation of transmitCANTask */
  transmitCANTaskHandle = osThreadNew(transmitCAN, NULL, &transmitCANTask_attributes);

  /* creation of controlACTTask */
  controlACTTaskHandle = osThreadNew(controlACT, NULL, &controlACTTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_monitorEBS */
/**
  * @brief  Function implementing the monitorEBSTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_monitorEBS */
void monitorEBS(void *argument)
{
  /* USER CODE BEGIN monitorEBS */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END monitorEBS */
}

/* USER CODE BEGIN Header_monitorWD */
/**
* @brief Function implementing the monitorWDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_monitorWD */
void monitorWD(void *argument)
{
  /* USER CODE BEGIN monitorWD */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(WDI_GPIO_Port, WDI_Pin);
    osDelay(500);
  }
  /* USER CODE END monitorWD */
}

/* USER CODE BEGIN Header_transmitCAN */
/**
* @brief Function implementing the transmitCANTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_transmitCAN */
void transmitCAN(void *argument)
{
  /* USER CODE BEGIN transmitCAN */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END transmitCAN */
}

/* USER CODE BEGIN Header_controlACT */
/**
* @brief Function implementing the controlACTTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_controlACT */
void controlACT(void *argument)
{
  /* USER CODE BEGIN controlACT */
  HAL_GPIO_WritePin(ACT1_En_GPIO_Port, ACT1_En_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ACT2_En_GPIO_Port, ACT2_En_Pin, GPIO_PIN_RESET);
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(ACT1_En_GPIO_Port, ACT1_En_Pin);
	HAL_GPIO_TogglePin(ACT2_En_GPIO_Port, ACT2_En_Pin);

	int act1 = HAL_GPIO_ReadPin(ACT1_En_GPIO_Port, ACT1_En_Pin);
	int act2 = HAL_GPIO_ReadPin(ACT2_En_GPIO_Port, ACT2_En_Pin);
	int sd_state = HAL_GPIO_ReadPin(SD_Out_GPIO_Port, SD_Out_Pin);
	char pBuf[64];
	int len = sprintf(pBuf, "ACT1: %d\nACT2: %d\n", act1, act2);
	HAL_UART_Transmit(&huart1, pBuf, len, 100);
    osDelay(1000);
  }
  /* USER CODE END controlACT */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

