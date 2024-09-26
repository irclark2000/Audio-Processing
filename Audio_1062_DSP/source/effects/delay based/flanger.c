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

#define MAX_FLANGER_DEPTH 30.0f
#define MAX_FLANGER_LFO_FREQ 0.5f

void setDelayMSec_FLANGER(FLANGER *flanger, float delayMSec);
static float tmp_out;  // use to avoid variable allocations inside functions

void intialize_FLANGER (FLANGER *flanger, float *buf, uint32_t buf_size,
		float delayMSec, float lfo_freq, float lfo_depth,
		float feedback_level, float wet_dry, float sampleRate) {
	setFeedback_level_FLANGER(flanger, feedback_level);
	initialize_variable_delay (&(flanger->vDelay), buf, buf_size, sampleRate);
	//setDelayMSec_FLANGER(flanger, delayMSec);
	setBaseDelayMSec(flanger, delayMSec);
	initialize_MIXER (&(flanger->mixer), wet_dry);
	initialize_LOWFREQOSC (&(flanger->lfo), lfo_depth, lfo_freq, sampleRate);
}

void setBaseDelayMSec(FLANGER *flanger, float delayMSec) {
	flanger->baseDelay_MSec = delayMSec;
	setDelayMSec_FLANGER(flanger, delayMSec);
}
void setDelayMSec_FLANGER(FLANGER *flanger, float delayMSec) {
	setDelay_VARDELAY((&flanger->vDelay), delayMSec / 1000.0f);
}
void setFeedback_level_FLANGER (FLANGER *flanger, float level) {
	tmp_out = (level < 0.0f) ? 0.0f : (level > 1.0f) ? 1.0f : level;
	flanger->feedback_level = tmp_out;
}
void setLFO_Frequency_FLANGER (FLANGER *flanger, float lfo_freq) {
	tmp_out = (lfo_freq < 0.01) ? 0.01 : (lfo_freq > MAX_FLANGER_LFO_FREQ) ? MAX_FLANGER_LFO_FREQ : lfo_freq;
	setFreq_LOWFREQOSC(&(flanger->lfo), tmp_out);
}

void setLFO_Depth_FLANGER (FLANGER *flanger, float lfo_depth) {
	tmp_out = (lfo_depth < 0.0f) ? 0.0 : (lfo_depth > MAX_FLANGER_DEPTH) ? MAX_FLANGER_DEPTH : lfo_depth;
	setAmplitude_LOWFREQOSC(&(flanger->lfo), tmp_out);
}
void setWetDry_FLANGER (FLANGER *flanger, float wet_dry) {
	setWetDry_MIXER (&(flanger->mixer), wet_dry);
}
static float var_MSec;
static float wet;
float update_FLANGER (FLANGER *flanger, float input) {
	update_LOWFREQOSC(&(flanger->lfo));
	var_MSec = (flanger->lfo.amplitude + flanger->lfo.out / 2000.0f);

	setDelayMSec_FLANGER(flanger, var_MSec + flanger->baseDelay_MSec);

	wet = getDelayedSample_VARDELAY(&(flanger->vDelay), input, flanger->feedback_level);
	flanger->out = applyWetDry_MIXER (&(flanger->mixer), wet, input);

	return flanger->out;
}


