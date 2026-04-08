/*
 * globals.c
 *
 *  Created on: Apr 7, 2026
 *      Author: peste
 */

#include "globals.h"

volatile uint16_t g_brakeWindow[ROLLING_WINDOW_SIZE] = {0};
volatile uint16_t g_tankWindow[ROLLING_WINDOW_SIZE] = {0};
volatile uint16_t g_brakeAvg = 0;
volatile uint16_t g_tankAvg = 0;

volatile uint8_t g_uartTxBusy = 0;
uint8_t g_uartTxBuf[UART_TX_SIZE] = {0};
