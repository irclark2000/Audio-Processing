/*
 * low_frequency_oscillator.c
 *
 *  Created on: Sep 23, 2024
 *      Author: isaac
 *
 */

#include <components/low_frequency_oscillator.h>
#include "effects_macros.h"

void setPhase_LOWFREQOSC(LOWFREQOSC *osc, float phaseAngle);

void initialize_LOWFREQOSC(LOWFREQOSC *osc, float amplitude, float minAmp,
		float maxAmp, float osc_freq, float minFreq, float maxFreq,
		float phaseAngle, float sampleFreq) {
	osc->sampleRate = sampleFreq;
	osc->direction = 1.0f;
	osc->counter = 0.0f;
	osc->amplitude_limits.minimum = minAmp;
	osc->amplitude_limits.maximum = maxAmp;
	if (minFreq < 0.01f) {
		minFreq = 0.01f;
	}
	if (maxFreq < 0.0f) {
		maxFreq = 0.5f * sampleFreq;
	}
	osc->frequency_limits.minimum = minFreq;
	osc->frequency_limits.maximum = maxFreq;
	setFreq_LOWFREQOSC(osc, osc_freq);
	setPhase_LOWFREQOSC(osc, phaseAngle);
	setAmplitude_LOWFREQOSC(osc, amplitude);
}

void setFreq_LOWFREQOSC(LOWFREQOSC *osc, float osc_freq) {
	if (osc_freq <= osc->frequency_limits.minimum) {
		osc_freq = osc->frequency_limits.minimum;
	} else if (osc_freq > osc->frequency_limits.maximum) {
		osc_freq = osc->frequency_limits.maximum;
	}
	osc->countLimit = 0.25f * (osc->sampleRate / osc_freq);

	// may have to adjust the count to keep it in bounds
	if (osc->counter > osc->countLimit) {
		osc->counter = osc->countLimit;
	} else if (osc->counter < -osc->countLimit) {
		osc->counter = -osc->countLimit;
	}
}
void setAmplitude_LOWFREQOSC(LOWFREQOSC *osc, float amplitude) {
	amplitude = MIN_MAX(amplitude, osc->amplitude_limits.minimum, osc->amplitude_limits.maximum);
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
// must set frequency first not useful if frequency is varied...
void setPhase_LOWFREQOSC(LOWFREQOSC *osc, float phaseAngle) {
    while (phaseAngle >= 180.0f) phaseAngle -= 360;
    while (phaseAngle <= -180.0f) phaseAngle += 360;
	phaseAngle = MIN_MAX(phaseAngle, -180.0f, 180.0f);
	if (phaseAngle >= 90.0f) {
		osc->direction = -1.0f;
		osc->counter = 1.0f + (90.0f - phaseAngle) / 90.0f;
	} else if (phaseAngle >= 0.0f) {
		osc->direction = 1.0f;
		osc->counter = (phaseAngle / 90.0f);
	} else if (phaseAngle >= -90.0f) {
		osc->direction = 1.0f;
		osc->counter = -1.0f  + (phaseAngle + 90.0) / 90.0f;
	} else { // -180 to -90
		osc->direction = -1.0f;
		osc->counter = -(phaseAngle + 180.0) / 90.0f;
	}
}
