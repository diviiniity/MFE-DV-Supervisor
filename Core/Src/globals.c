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
