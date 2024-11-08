/*
 * variable_bandpass_filter.c
 *
 *  Created on: Oct 1, 2024
 *      Author: isaac
 *      Uses a second order all pass to provide either band pass or band stop filter
 *      with setable Q and center frequency
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

#include "variable_bandpass_filter.h"

void initialize_VARBANDPASS(VARBANDPASS *vbf, float centerFreq, float Q, float sampleRate, uint8_t bandPass) {
	vbf->pass_stop = bandPass;
	initialize_SECONDALLPASSFILTER(&(vbf->apf), centerFreq, Q, sampleRate);
}

float update_VARBANDPASS(VARBANDPASS *vbf, float input) {
	vbf->out = update_SECONDALLPASSFILTER(&(vbf->apf), input);
	vbf->out = (input + ((vbf->pass_stop != 0) ? -vbf->out : vbf->out)) * 0.5f;
	return vbf->out;
}
void gui_setCenterFrequency_VARBANDPASS(VARBANDPASS *vbf) {
	setCenterFrequency_SECONDALLPASSFILTER(&(vbf->apf), vbf->gui_Freq, vbf->gui_Q);
}
void setCenterFrequency_VARBANDPASS(VARBANDPASS *vbf, float centerFreq, float Q) {
	setCenterFrequency_SECONDALLPASSFILTER(&(vbf->apf), centerFreq, Q);
}
