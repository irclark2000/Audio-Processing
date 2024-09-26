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
	float baseDelayMSec;
	float output;
} CHORUSELEMENT;

typedef struct {
	float freq;
	float depth;
} CELEMENTDATA;

void initialize_CHORUSELEMENT(CHORUSELEMENT * cEl, float * buf, uint32_t buf_size, float delayMSec,
		CELEMENTDATA cdata, float sampleRate);
void setBaseDelayMSec_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec);
void setDepth_CHORUSELEMENT(CHORUSELEMENT * cEl, float depth);
void setFrequency_CHORUSELEMENT(CHORUSELEMENT * cEl, float freq);
float update_CHORUSELEMENT (CHORUSELEMENT * cEl, float input);
#endif /* EFFECTS_DELAY_BASED_CHORUS_ELEMENT_H_ */
