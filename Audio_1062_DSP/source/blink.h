/*
 * blink.h
 *
 *  Created on: Aug 25, 2024
 *      Author: isaac
 */

#ifndef BLINK_H_
#define BLINK_H_
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "board.h"

#define EXAMPLE_LED_GPIO     BOARD_USER_LED_GPIO
#define EXAMPLE_LED_GPIO_PIN BOARD_USER_LED_GPIO_PIN


#if defined(__cplusplus)
extern "C" {
#endif

void LED_InitPins(void);
void initializeLED(void);
void toggleLED(void);
#if defined(__cplusplus)
}
#endif
#endif /* BLINK_H_ */
