/*
 * state_variable_filter.h
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

#ifndef EFFECTS_COMPONENTS_STATE_VARIABLE_FILTER_H_
#define EFFECTS_COMPONENTS_STATE_VARIABLE_FILTER_H_
// includes low pass, high pass, and bandpass outputs

#include <math.h>

typedef struct {
	float yb[2];
	float yl[2];
	float yh[2];

	float yhOut;
	float ybOut;
	float ylOut;

	float damping;
	float sampleRate;
	float sampleTime;
	float centerFreq;

	float F1;
	float Q1;

}SVFILTER;

void intialize_SVFILTER (SVFILTER *svf, float centerFreq, float damping, float sampleRate);
void set_frequency_damping_SVFILTER(SVFILTER *svf, float centerFreq, float damping);
void gui_set_frequency_damping_SVFILTER(SVFILTER *svf);
void apply_SVFILTER (SVFILTER *svf, float input);

#endif /* EFFECTS_COMPONENTS_STATE_VARIABLE_FILTER_H_ */
