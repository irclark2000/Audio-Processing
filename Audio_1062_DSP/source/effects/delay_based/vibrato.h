/*
 * vibrato.h
 *
 *  Created on: Sep 30, 2024
 *      Author: isaac
 */
/*
Copyright 2024 Isaac R. Clark, Jr.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef EFFECTS_DELAY_BASED_VIBRATO_H_
#define EFFECTS_DELAY_BASED_VIBRATO_H_

#include <stdint.h>
#include "components/mixer.h"
#include "components/variable_delay.h"
#include "components/low_frequency_oscillator.h"
#include "components/components.h"

typedef struct {
	LOWFREQOSC lfo;
	PARAMETER_LIMITS lfo_freq_limits;
	VARDELAY vDelay;
	float baseDelayMSec;
	PARAMETER_LIMITS delay_limitsMsec;
	MIXER mixer;
	float out;
} VIBRATO;

void initialize_VIBRATO (VIBRATO * vb, float *buf, float buf_size,
		float base_vibrato_frequency, float delayMSec, float variation_delayMSec, float sampleRate);
void setWetDry_VIBRATO (VIBRATO * vb, float wet_dry);
void setFreq_VIBRATO(VIBRATO * vb, float osc_freq);
void setBaseDelayMSec_VIBRATO (VIBRATO * vb, float delayMSec);
void setAmplitude_VIBRATO(VIBRATO * vb, float amplitude);
float apply_VIBRATO (VIBRATO * vb, float input);
#endif /* EFFECTS_DELAY_BASED_VIBRATO_H_ */
