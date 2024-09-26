/*
 * flanger.h
 *
 *  Created on: Sep 24, 2024
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

#ifndef EFFECTS_DELAY_BASED_FLANGER_H_
#define EFFECTS_DELAY_BASED_FLANGER_H_

#include "variable_delay.h"
#include "mixer.h"
#include "low_frequency_oscillator.h"

typedef struct {
	VARDELAY vDelay;
	float baseDelay_MSec; // delay varies above base using lfo
	LOWFREQOSC lfo;  // used to vary the flanger delay
	float feedback_level;
	float out;
	MIXER mixer;
} FLANGER;

void intialize_FLANGER (FLANGER *flanger, float *buf, uint32_t buf_size,
		float delayMSec, float lfo_freq, float lfo_depth,
		float feedback_level, float wet_dry, float sampleRate);
void setBaseDelayMSec(FLANGER *flanger, float delayMSec);

void setFeedback_level_FLANGER (FLANGER *flanger, float level);
void setLFO_Frequency_FLANGER (FLANGER *flanger, float lfo_freq);
void setLFO_Depth_FLANGER (FLANGER *flanger, float lfo_freq);
void setWetDry_FLANGER (FLANGER *flanger, float wet_dry);
float update_FLANGER (FLANGER *flanger, float input);


#endif /* EFFECTS_DELAY_BASED_FLANGER_H_ */
