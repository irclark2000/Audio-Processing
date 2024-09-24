/*
 * low_frequency_oscillator.c
 *
 *  Created on: Sep 23, 2024
 *      Author: isaac
 */

#include "low_frequency_oscillator.h"

void initialize_LOWFREQOSC (LOWFREQOSC *osc, float amplitude, float osc_freq, float sampleFreq) {
	osc->amplitude = amplitude;
	osc->sampleRate = sampleFreq;
	osc->direction = 1.0f;
	osc->counter = 0.0f;
	setFreq_LOWFREQOSC(osc, osc_freq);
}

void setFreq_LOWFREQOSC(LOWFREQOSC *osc, float osc_freq) {
	if (osc_freq <= 0.0f) {
		osc_freq = 1.0f;
	}
	else if (osc_freq > 0.5f * osc->sampleRate) {
		osc_freq = 0.5f * osc->sampleRate;
	}
	osc->countLimit = 0.25f * (osc->sampleRate / osc_freq);

	// may have to adjust the count to keep it in bounds
	if (osc->counter > osc->countLimit) {
		osc->counter = osc->countLimit;
	}
	else if (osc->counter < -osc->countLimit)  {
		osc->counter = -osc->countLimit;
	}
}

float getOutput_LOWFREQOSC(LOWFREQOSC *osc) {
	float output = osc->amplitude * osc->counter / osc->countLimit;

	// update the direction if needed
	if (osc->counter >= osc->countLimit) {
		osc->direction = -1.0f;
	}
	if (osc->counter <= -osc->countLimit) {
		osc->direction = 1.0f;
	}
	// bump the count
	osc->counter += osc->direction;
	return output;
}

