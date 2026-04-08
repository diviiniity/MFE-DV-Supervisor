/*
 * globals.h
 *
 *  Created on: Apr 7, 2026
 *      Author: peste
 */

#ifndef INC_GLOBALS_H_
#define INC_GLOBALS_H_

#include <stdint.h>

#define ROLLING_WINDOW_SIZE 100
#define UART_TX_SIZE    64

extern volatile uint16_t g_brakeWindow[ROLLING_WINDOW_SIZE];
extern volatile uint16_t g_tankWindow[ROLLING_WINDOW_SIZE];
extern volatile uint16_t g_brakeAvg;
extern volatile uint16_t g_tankAvg;

extern volatile uint8_t g_uartTxBusy;
extern uint8_t g_uartTxBuf[UART_TX_SIZE];

#endif /* INC_GLOBALS_H_ */

