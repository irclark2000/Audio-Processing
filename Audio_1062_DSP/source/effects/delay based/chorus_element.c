/*
 * chorus_element.c
 *
 *  Created on: Sep 24, 2024
 *      Author: isaac
 */


#include "chorus_element.h"
#include <math.h>

void setdelayMS_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec);

void initialize_CHORUSELEMENT(CHORUSELEMENT * cEl, float * buf, uint32_t buf_size, float delayMSec,
		CELEMENTDATA cData, float sampleRate) {
	initialize_variable_delay (&(cEl->vDelay), buf, buf_size, sampleRate);
	setBaseDelayMSec_CHORUSELEMENT(cEl, delayMSec);
	initialize_LOWFREQOSC (&(cEl->lfo), cData.depth, cData.freq, sampleRate);
}

void setBaseDelayMSec_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec) {
	cEl->baseDelayMSec = delayMSec;
}

void setdelayMS_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec) {
	setDelay_VARDELAY(&(cEl->vDelay), delayMSec / 1000.0f);
}

void setDepth_CHORUSELEMENT(CHORUSELEMENT * cEl, float depth) {
	setAmplitude_LOWFREQOSC(&(cEl->lfo), depth);

}
void setFrequency_CHORUSELEMENT(CHORUSELEMENT * cEl, float freq) {
	setFreq_LOWFREQOSC(&(cEl->lfo), freq);
}

float update_CHORUSELEMENT (CHORUSELEMENT * cEl, float input) {
	// update lfo first, then set the delay
	// no feedback with the delay component
	update_LOWFREQOSC(&(cEl->lfo));
	float var = fabs(cEl->lfo.out) / 1000.0 + cEl->baseDelayMSec;
	setdelayMS_CHORUSELEMENT(cEl, var);
	// 0.0f in fb_level parameter as feedback is not used in chorus effect
	cEl->output = getDelayedSample_VARDELAY(&(cEl->vDelay), input, 0.0f);

	return cEl->output;
}
