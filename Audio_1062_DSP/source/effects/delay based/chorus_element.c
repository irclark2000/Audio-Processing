/*
 * chorus_element.c
 *
 *  Created on: Sep 24, 2024
 *      Author: isaac
 */


#include "chorus_element.h"
#include <math.h>

void setdelayMS_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec);

void initialize_CHORUSELEMENT(CHORUSELEMENT * cEl, float * buf, uint32_t buf_size, float delayMSec, float amplitude, float freq, float sampleRate) {
	initialize_LOWFREQOSC (&(cEl->lfo), amplitude, freq, sampleRate);
	initialize_variable_delay (&(cEl->vDelay), buf, buf_size, sampleRate);
	setBaseDelayMS_CHORUSELEMENT(cEl, delayMSec);
}

void setBaseDelayMS_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec) {
	cEl->baseDelayMS = delayMSec;
	setDelayMS_CHORUSELEMENT(cEl, delayMSec);
}

void setdelayMS_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec) {
	setDelay_VARDELAY(&(cEl->vDelay), delayMSec / 1000.0f);
}
float update_CHORUSELEMENT (CHORUSELEMENT * cEl, float input) {
	// update lfo first, the set the delay
	// get sample should be called before adding new sample to buffer
	// add feedback (with gain) to the input
	update_LOWFREQOSC(&(cEl->lfo));
	float var = fabs(cEl->lfo.out) / 1000.0 + cEl->baseDelayMS;
	setdelayMS_CHORUSELEMENT(cEl, var);
	cEl->output = getDelayedSample_VARDELAY(&(cEl->vDelay), input, 0.0f);

	return cEl->output;
}
