/*
 * low_frequency_oscillator.h
 *
 *  Created on: Sep 23, 2024
 *      Author: isaac
 */

#ifndef LOW_FREQUENCY_OSCILLATOR_H_
#define LOW_FREQUENCY_OSCILLATOR_H_

#include <stdint.h>
// generate low frequency triangle wave

typedef struct {
	float sampleRate;
	float countLimit;
	float amplitude;
	float counter;
	float direction;
} LOWFREQOSC;

void initialize_LOWFREQOSC (LOWFREQOSC *osc, float amplitude, float osc_freq, float sampleFreq);
void setFreq_LOWFREQOSC(LOWFREQOSC *osc, float osc_freq);
float getOutput_LOWFREQOSC(LOWFREQOSC *osc);
#endif /* LOW_FREQUENCY_OSCILLATOR_H_ */
