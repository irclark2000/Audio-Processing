/*
 * flanger.c
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

#include "flanger.h"
#include "components/effects_macros.h"

#define MAX_FLANGER_DEPTH_MSEC 30.0f
#define MAX_FLANGER_LFO_FREQ 5.0f

void setDelayMSec_FLANGER(FLANGER *flanger, float delayMSec);

void intialize_FLANGER (FLANGER *flanger, float *buf, uint32_t buf_size,
		float delayMSec, float lfo_freq, float lfo_depth,
		float feedback_level, float wet_dry, float sampleRate) {
	setFeedback_level_FLANGER(flanger, feedback_level);
	initialize_variable_delay (&(flanger->vDelay), buf, buf_size, sampleRate);
	setBaseDelayMSec_FLANGER(flanger, delayMSec);
	initialize_MIXER (&(flanger->mixer), wet_dry);
	initialize_LOWFREQOSC (&(flanger->lfo),
			lfo_depth, 0.0f, 30.0f,
			lfo_freq, 0.01f, 3.0f, 0.0f,
			1,  // sinusoidal output
			sampleRate);
}

void setBaseDelayMSec_FLANGER(FLANGER *flanger, float delayMSec) {
	flanger->baseDelay_MSec = delayMSec;
}
void setDelayMSec_FLANGER(FLANGER *flanger, float delayMSec) {
	setDelay_VARDELAY((&flanger->vDelay), delayMSec / 1000.0f);
}
void setFeedback_level_FLANGER (FLANGER *flanger, float level) {
	flanger->feedback_level = MIN_MAX(level, 0.0f, 0.98f);
}
void setLFO_Frequency_FLANGER (FLANGER *flanger, float lfo_freq) {
	lfo_freq = MIN_MAX(lfo_freq, 0.01f, MAX_FLANGER_LFO_FREQ);
	setFreq_LOWFREQOSC(&(flanger->lfo), lfo_freq);
}

void setLFO_Depth_FLANGER (FLANGER *flanger, float lfo_depth) {
	lfo_depth = MIN_MAX (lfo_depth, 0.0f, MAX_FLANGER_DEPTH_MSEC);
	setAmplitude_LOWFREQOSC(&(flanger->lfo), lfo_depth);
}
void setWetDry_FLANGER (FLANGER *flanger, float wet_dry) {
	setWetDry_MIXER (&(flanger->mixer), wet_dry);
}
static float var_MSec;
float update_FLANGER (FLANGER *flanger, float input) {
	update_LOWFREQOSC(&(flanger->lfo));
	var_MSec = (flanger->lfo.amplitude + flanger->lfo.out) / 2.0f;
	setDelayMSec_FLANGER(flanger, var_MSec + flanger->baseDelay_MSec);
	flanger->out = getDelayedSample_VARDELAY(&(flanger->vDelay), input, flanger->feedback_level);
	flanger->out = applyWetDry_MIXER (&(flanger->mixer), flanger->out, input);

	return flanger->out;
}


