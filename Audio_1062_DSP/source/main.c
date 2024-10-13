/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_debug_console.h"

#include "audio_wm8960.h"
#include "dma_functions.h"
#include "boardStartup.h"
#include "potentiometer.h"
#include "blink.h"
#include "process.h"
#include "updateSettings.h"
#include "core_cm7.h"    // required for processor cycle counting

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    board_startup();
    initialize_dma_system();
    setup_SAI_EDMA();
    potentiometerReadInit();
    initializeEffects(AUD_AUDIO_SAMPLE_RATE);
    initializeLED();

    uint32_t result = 0;
    uint16_t timer_counter = 0;

    int count = 0;
    toggleLED();
    // main processing loop
    while (1)
    {
    	// using single half-complete interrupt with SAI
    	if (g_Transfer_Done) {
    		//memcpy(g_bufOut, g_bufIn, BUFFER_SIZE/2);
    		// enable processor cycle counting
    		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // enable trace for DWT features
    		DWT->LAR = 0xc5acce55; // unlock access to DWT registers
    		DWT->CYCCNT = 0;
    		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // enable the cycle counter
    		processHalf(g_bufIn, g_bufOut,
    				HALF_BUFFER_SIZE, STEREO,
    				AUD_AUDIO_SAMPLE_RATE);
    		result += DWT->CYCCNT;  // add cycle count to result
    		timer_counter++;
    		g_Transfer_Done = false;

    		if (timer_counter % 100 == 0 && timer_counter > 0 && timer_counter < 1100) {
    			PRINTF("processHalf took %d cycles.\r\n", result);
    			result = 0;
    		}
    	}
    	// new potentiometer readings are available
    	else if (g_AdcConversionDoneFlag) {
    		if (count == 100) {  // slow down the led blinking by factor of 100
    			toggleLED();
    			count = 0;
    		}
    		count++;
    		updateSettings();
#if 0
            PRINTF("Readings: %d %d %d %d %d %d\r\n",
            		(int)(4096 *getPotentiometerValue(1)),
            		(int)(4096 *getPotentiometerValue(2)),
            		(int)(4096 *getPotentiometerValue(3)),
            		(int)(4096 *getPotentiometerValue(4)),
            		(int)(4096 *getPotentiometerValue(5)),
            		(int)(4096 *getPotentiometerValue(6)));
#endif
    		g_AdcConversionDoneFlag = false;
    	}
    }
}
