/*
 * main.h
 *
 *  Created on: Aug 14, 2024
 *      Author: isaac
 */

#ifndef BOARDSTARTUP_H_
#define BOARDSTARTUP_H_

#include <audio_wm8960.h>
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
#endif /* BOARDSTARTUP_H_ */
