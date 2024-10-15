/*
 * chorus_element.c
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


#include <delay_based/chorus_element.h>
#include <math.h>

void setdelayMS_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec);

void initialize_CHORUSELEMENT(CHORUSELEMENT * cEl, float * buf, uint32_t buf_size, float delayMSec,
		CELEMENTDATA cData, float sampleRate) {
	initialize_variable_delay (&(cEl->vDelay), buf, buf_size, sampleRate);
	setBaseDelayMSec_CHORUSELEMENT(cEl, delayMSec);
	initialize_LOWFREQOSC (&(cEl->lfo), cData.depth, 0.0f, 30.0f, cData.freq, 0.01f, -1.0f, 0.0f, 0, sampleRate);
}

void setBaseDelayMSec_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec) {
	cEl->baseDelayMSec = delayMSec;
}

void setdelayMS_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec) {
	setDelay_VARDELAY(&(cEl->vDelay), delayMSec / 1000.0f);
}

void setDepth_CHORUSELEMENT(CHORUSELEMENT * cEl, float depth) {
	setAmplitude_LOWFREQOSC(&(cEl->lfo), depth);

}
void setFrequency_CHORUSELEMENT(CHORUSELEMENT * cEl, float freq) {
	setFreq_LOWFREQOSC(&(cEl->lfo), freq);
}

float update_CHORUSELEMENT (CHORUSELEMENT * cEl, float input) {
	// update lfo first, then set the delay
	// no feedback with the delay component
	update_LOWFREQOSC(&(cEl->lfo));
	float var = fabs(cEl->lfo.out) / 1000.0 + cEl->baseDelayMSec;
	setdelayMS_CHORUSELEMENT(cEl, var);
	// 0.0f in fb_level parameter as feedback is not used in chorus effect
	cEl->output = getDelayedSample_VARDELAY(&(cEl->vDelay), input, 0.0f);

	return cEl->output;
}
