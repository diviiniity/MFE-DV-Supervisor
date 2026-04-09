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
#include "adc.h"
#include "fdcan.h"

#include "globals.h"

#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BRAKE_OK_THRESHOLD 1300
#define TANK_OK_THRESHOLD 1300
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

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void monitorEBS(void *argument);
void monitorWD(void *argument);
void transmitCAN(void *argument);

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
  static uint16_t writeIndex = 0;
  static uint16_t sampleCount = 0;
  static uint32_t brakeRunningSum = 0;
  static uint32_t tankRunningSum = 0;
  /* Infinite loop */
  for(;;)
  {

	if (sampleCount < ROLLING_WINDOW_SIZE) sampleCount++;
	else {
		brakeRunningSum -= g_brakeWindow[writeIndex];
		tankRunningSum -= g_tankWindow[writeIndex];
	}

	uint16_t brake = g_brakeAdcLatest;
	uint16_t tank = g_tankAdcLatest;

	g_brakeWindow[writeIndex] = brake;
	g_tankWindow[writeIndex] = tank;

	brakeRunningSum += brake;
	tankRunningSum += tank;

	writeIndex = (writeIndex + 1) % ROLLING_WINDOW_SIZE;

    g_brakeAvg = (uint16_t)(brakeRunningSum / sampleCount);
    g_tankAvg = (uint16_t)(tankRunningSum / sampleCount);



    g_adcErrorActive = (g_brakeAvg < BRAKE_OK_THRESHOLD || g_tankAvg < TANK_OK_THRESHOLD) ? 1U : 0U;
    checkErrors();

    if (!g_uartTxBusy) {
    	int len = snprintf((char*)g_uartTxBuf, UART_TX_SIZE,
    	        "P:%4u(%d) E:%4u(%d) | TS:%d | WDO:%d WDI:%d EN:%d | RTD:%d SDC:%d A1:%d A2:%d\r\n",
				g_brakeAvg, 1,
				g_tankAvg,   1,
    	        1,
    	        1, 1, 1,
    	        1, 1, 1, 1
    	    );
    	if (len > 0 && len < UART_TX_SIZE) {
    	    g_uartTxBusy = 1;
    	    if (HAL_UART_Transmit_DMA(&huart1, g_uartTxBuf, (uint16_t)len) != HAL_OK) {
    	        g_uartTxBusy = 0;
    	    }
    	}
    }
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
  FDCAN_TxHeaderTypeDef txHeader = {0};
  uint8_t txData[8] = {0};

  txHeader.Identifier = 0x01;
  txHeader.IdType = FDCAN_STANDARD_ID;
  txHeader.TxFrameType = FDCAN_DATA_FRAME;
  txHeader.DataLength = FDCAN_DLC_BYTES_8;
  txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  txHeader.BitRateSwitch = FDCAN_BRS_OFF;
  txHeader.FDFormat = FDCAN_CLASSIC_CAN;
  txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  txHeader.MessageMarker = 0;

  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
	Error_Handler();
  }
  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
	Error_Handler();
  }
  /* Infinite loop */
  for(;;)
  {
	  txData[0] = (uint8_t)HAL_GPIO_ReadPin(SD_Status_GPIO_Port, SD_Status_Pin);
	  txData[1] = (uint8_t)(
	  		((uint8_t)HAL_GPIO_ReadPin(ACT1_En_GPIO_Port, ACT1_En_Pin) & 0x01U) |
	  		((((uint8_t)HAL_GPIO_ReadPin(ACT2_En_GPIO_Port, ACT2_En_Pin) & 0x01U) << 1U))
	  );

	  txData[2] = (uint8_t)(g_brakeAvg & 0xFFU);
	  txData[3] = (uint8_t)((g_brakeAvg >> 8U) & 0xFFU);
	  txData[4] = (uint8_t)(g_tankAvg & 0xFFU);
	  txData[5] = (uint8_t)((g_tankAvg >> 8U) & 0xFFU);

	  txData[6] = (uint8_t)HAL_GPIO_ReadPin(RTD_GPIO_Port, RTD_Pin);
	  txData[7] = (uint8_t)(HAL_GPIO_ReadPin(WDO_GPIO_Port, WDO_Pin) == GPIO_PIN_RESET);

	  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, txData) != HAL_OK) {
	  	Error_Handler();
	  }

	  osDelay(10);
  }
  /* USER CODE END transmitCAN */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

