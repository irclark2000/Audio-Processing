/*
 * wah_wah.c
 *
 *  Created on: Oct 3, 2024
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


#include <delay_based/wah_wah.h>
void initialize_WAHWAH(WAHWAH * ww, float wahFreq, float minCutoffFreq, float maxCutoffFreq, float damp, float wet_dry, float sampleRate) {
	ww->cutoff_limits.maximum = maxCutoffFreq;
	ww->cutoff_limits.minimum = minCutoffFreq;
	initialize_LOWFREQOSC (&(ww->lfo), maxCutoffFreq - minCutoffFreq, 0.0f,
			5000.0f, wahFreq / 2.0f, 0.01f, -1.0f, 0.0f, 0, sampleRate);
	intialize_SVFILTER (&(ww->svf), minCutoffFreq, damp, sampleRate);
	initialize_MIXER (&(ww->mixer), wet_dry);
}

float apply_WAHWAH (WAHWAH * ww, float input) {
	update_LOWFREQOSC(&(ww->lfo));
	// first step should be to set the cutoff frequency for the filter
	set_frequency_damping_SVFILER(&(ww->svf), ww->cutoff_limits.minimum + fabs(ww->lfo.out), -1);
	apply_SVFILTER (&(ww->svf), input);
	applyWetDry_MIXER (&(ww->mixer), ww->svf.ybOut, input);
 	ww->out =  ww->mixer.out;
	return ww->out;
}

void setMInMAXCutoffFrequency (WAHWAH *ww, float minCutoffFreq, float maxCutoffFreq) {
	minCutoffFreq = (minCutoffFreq < 0.0f) ? ww->cutoff_limits.minimum : minCutoffFreq;
	maxCutoffFreq = (maxCutoffFreq < 0.0f) ? ww->cutoff_limits.maximum : maxCutoffFreq;
	ww->cutoff_limits.minimum = minCutoffFreq;
	ww->cutoff_limits.maximum = maxCutoffFreq;
	set_frequency_damping_SVFILER(&(ww->svf), ww->cutoff_limits.minimum, -1);
	setAmplitude_LOWFREQOSC(&(ww->lfo), ww->cutoff_limits.maximum - ww->cutoff_limits.minimum);
}

void setWahFreq_Damping_WAHWAH(WAHWAH * ww, float wahFreq, float damp) {
	// when we take the absolute value that doubles the lfo frequency
	setFreq_LOWFREQOSC(&(ww->lfo), wahFreq / 2.0f);
	set_frequency_damping_SVFILER(&(ww->svf), -1.0f, damp);
}

