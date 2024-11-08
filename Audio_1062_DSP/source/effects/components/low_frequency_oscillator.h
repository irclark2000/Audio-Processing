/*
 * low_frequency_oscillator.h
 *
 *  Created on: Sep 23, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_COMPONENTS_LOW_FREQUENCY_OSCILLATOR_H_
#define EFFECTS_COMPONENTS_LOW_FREQUENCY_OSCILLATOR_H_

#include <stdint.h>
#include <math.h>
#include "components.h"

static const float PI = 3.14159265359f;
// generate low frequency triangle wave

typedef struct {
	float sampleRate;
	float sampleTime;
	float countLimit;
	float wcT;
	float oscFreq;
	float amplitude;
	PARAMETER_LIMITS amplitude_limits;
	PARAMETER_LIMITS frequency_limits;
	float phaseAngleDeg;
	float phaseRadians;
	uint8_t triangle_sine_select;   // 0 = triangle, 1 = sine

	float counter;
	float direction;
	float out;
} LOWFREQOSC;

void initialize_LOWFREQOSC (LOWFREQOSC *osc, float amplitude, float minAmp,
		float maxAmp, float osc_freq, float minFreq, float maxFreq,
		float phaseAngle, uint8_t outputSine, float sampleFreq);
void setFreq_LOWFREQOSC(LOWFREQOSC *osc, float osc_freq);
void gui_setFrequency(LOWFREQOSC *osc);
void setAmplitude_LOWFREQOSC(LOWFREQOSC *osc, float amplitude);
void update_LOWFREQOSC(LOWFREQOSC *osc);
#endif /* EFFECTS_COMPONENTS_LOW_FREQUENCY_OSCILLATOR_H_ */
