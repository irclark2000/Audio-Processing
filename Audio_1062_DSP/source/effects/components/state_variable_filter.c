/*
 * state_variable_filter.c
 *
 *  Created on: Oct 3, 2024
 *      Author: isaac
 *      state variable filter has first low pass, high pass, and band pass outputs.
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

#include "state_variable_filter.h"
static const float PI = 3.14159265359f;

void intialize_SVFILTER (SVFILTER *svf, float centerFreq, float damping, float sampleRate) {
	svf->sampleRate = sampleRate;
	svf->sampleTime = 1.0f / sampleRate;
	svf->yh[0] = svf->yh[1] = 0.0f;
	svf->yb[0] = svf->yb[1] = 0.0f;
	svf->yl[0] = svf->yl[1] = 0.0f;
	set_frequency_damping_SVFILER(svf, centerFreq, damping);
}
// if damping or frequency parameter is negative, then the old value will be retained.
void set_frequency_damping_SVFILER(SVFILTER *svf, float centerFreq, float damping) {
	svf->centerFreq = (centerFreq > 0) ? centerFreq : svf->centerFreq;
	svf->damping = (damping > 0) ? damping : svf->damping;
	svf->F1 = 2.0f * sinf(PI * svf->centerFreq * svf->sampleTime);
	svf->Q1 = 2.0f * svf->damping;
}

void apply_SVFILTER (SVFILTER *svf, float input) {
	svf->yh[1] = svf->yh[0];
	svf->yb[1] = svf->yb[0];
	svf->yl[1] = svf->yl[0];

	svf->yh[0] = input - svf->yl[1] - svf->Q1 * svf->yb[1];
	svf->yb[0] = svf->F1 * svf->yh[0] + svf->yb[1];
	svf->yl[0] = svf->F1 * svf->yb[0] + svf->yl[1];

	svf->yhOut = svf->yh[0];
	svf->ybOut = svf->yb[0];
	svf->ylOut = svf->yl[0];
}

