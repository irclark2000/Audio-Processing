/*
 * chorus_element.h
 *
 *  Created on: Sep 24, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_DELAY_BASED_CHORUS_ELEMENT_H_
#define EFFECTS_DELAY_BASED_CHORUS_ELEMENT_H_
#include <stdint.h>
#include "variable_delay.h"
#include "low_frequency_oscillator.h"

typedef struct {
	LOWFREQOSC lfo;
	VARDELAY vDelay;
	float baseDelayMS;
	float output;
} CHORUSELEMENT;

void initialize_CHORUSELEMENT(CHORUSELEMENT * ce, float * buf, uint32_t buf_size, float delayMSec,
		float amplitude, float freq, float sampleRate);
void setBaseDelayMS_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec);
float update_CHORUSELEMENT (CHORUSELEMENT * cEl, float input);
#endif /* EFFECTS_DELAY_BASED_CHORUS_ELEMENT_H_ */
