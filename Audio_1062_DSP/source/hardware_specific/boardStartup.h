/*
 * main.h
 *
 *  Created on: Aug 14, 2024
 *      Author: isaac
 */

#ifndef HARDWARE_SPECIFIC_BOARDSTARTUP_H_
#define HARDWARE_SPECIFIC_BOARDSTARTUP_H_

#include <hardware_specific/audio_wm8960.h>
#include "clock_config.h"
#include "board.h"


#if defined(__cplusplus)
extern "C" {
#endif

void board_startup(void);
void my_InitBootPins(void);

#if defined(__cplusplus)
}
#endif
#endif /* HARDWARE_SPECIFIC_BOARDSTARTUP_H_ */
