/*
 * low_frequency_oscillator.h
 *
 *  Created on: Sep 23, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_COMPONENTS_LOW_FREQUENCY_OSCILLATOR_H_
#define EFFECTS_COMPONENTS_LOW_FREQUENCY_OSCILLATOR_H_

#include <stdint.h>
// generate low frequency triangle wave

typedef struct {
	float sampleRate;
	float countLimit;
	float amplitude;
	float minFreq;
	float maxFreq;
	float minAmp;
	float maxAmp;

	float counter;
	float direction;
	float out;
} LOWFREQOSC;

void initialize_LOWFREQOSC (LOWFREQOSC *osc, float amplitude, float minAmp,
		float maxAmp, float osc_freq, float minFreq, float maxFreq, float sampleFreq);
void setFreq_LOWFREQOSC(LOWFREQOSC *osc, float osc_freq);
void setAmplitude_LOWFREQOSC(LOWFREQOSC *osc, float amplitude);
void update_LOWFREQOSC(LOWFREQOSC *osc);
#endif /* EFFECTS_COMPONENTS_LOW_FREQUENCY_OSCILLATOR_H_ */
