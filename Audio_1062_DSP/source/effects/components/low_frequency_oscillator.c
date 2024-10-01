/*
 * low_frequency_oscillator.c
 *
 *  Created on: Sep 23, 2024
 *      Author: isaac
 *
 */

#include <components/low_frequency_oscillator.h>
#include "effects_macros.h"

void initialize_LOWFREQOSC (LOWFREQOSC *osc, float amplitude, float minAmp,
		float maxAmp, float osc_freq, float minFreq, float maxFreq, float sampleFreq) {
	osc->sampleRate = sampleFreq;
	osc->direction = 1.0f;
	osc->counter = 0.0f;
	osc->minAmp = minAmp;
	osc->maxAmp = maxAmp;
	if (minFreq < 0.01f) {
		minFreq = 0.01f;
	}
	if (maxFreq < 0.0f) {
		maxFreq = 0.5f * sampleFreq;
	}
	setFreq_LOWFREQOSC(osc, osc_freq);
	setAmplitude_LOWFREQOSC(osc, amplitude);
}

void setFreq_LOWFREQOSC(LOWFREQOSC *osc, float osc_freq) {
	if (osc_freq <= osc->minFreq) {
		osc_freq = osc->minFreq;
	}
	else if (osc_freq > osc->maxFreq) {
		osc_freq = osc->maxFreq;
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
void setAmplitude_LOWFREQOSC(LOWFREQOSC *osc, float amplitude) {
	amplitude = MIN_MAX(amplitude, osc->minFreq, osc->maxFreq);
	osc->amplitude = amplitude;
}

// this function should be called in the update function of an effect using the lfo
void update_LOWFREQOSC(LOWFREQOSC *osc) {
	osc->out = osc->amplitude * osc->counter / osc->countLimit;

	// update the direction if needed
	if (osc->counter >= osc->countLimit) {
		osc->direction = -1.0f;
	}
	if (osc->counter <= -osc->countLimit) {
		osc->direction = 1.0f;
	}
	// bump the count
	osc->counter += osc->direction;
}

