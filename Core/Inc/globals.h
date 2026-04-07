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

extern volatile uint16_t g_brakeWindow[ROLLING_WINDOW_SIZE];
extern volatile uint16_t g_tankWindow[ROLLING_WINDOW_SIZE];
extern volatile uint16_t g_brakeAvg;
extern volatile uint16_t g_tankAvg;


#endif /* INC_GLOBALS_H_ */
