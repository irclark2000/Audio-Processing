/*
 * blink.h
 *
 *  Created on: Aug 25, 2024
 *      Author: isaac
 */

#ifndef HARDWARE_SPECIFIC_BLINK_H_
#define HARDWARE_SPECIFIC_BLINK_H_
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "board.h"

#define EXAMPLE_LED_GPIO     BOARD_USER_LED_GPIO
#define EXAMPLE_LED_GPIO_PIN BOARD_USER_LED_GPIO_PIN
#define GPIODEFAULTPINVALUE 0x10B0U

#if defined(__cplusplus)
extern "C" {
#endif

void LED_InitPins(void);
void initializeLED(void);
void toggleLED(void);
#if defined(__cplusplus)
}
#endif
#endif /* HARDWARE_SPECIFIC_BLINK_H_ */
