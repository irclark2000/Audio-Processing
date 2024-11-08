/*
 * variable_bandpass_filter.h
 *
 *  Created on: Oct 1, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_COMPONENTS_VARIABLE_BANDPASS_FILTER_H_
#define EFFECTS_COMPONENTS_VARIABLE_BANDPASS_FILTER_H_
/*
 *  Uses a 2nd order allpass filter to produce a bandpass filter with
 *  settable Q and center frequency.  You can easily produce a bandstop filter
 *  by setting the pass/stop parameter to 0 rather than 1
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

#include "filters/second_order_all_pass.h"

typedef struct {
	SECONDALLPASSFILTER apf;
	uint8_t pass_stop;   // 1 = pass, 0 = stop
	float gui_Q;
	float gui_Freq;

	float out;
} VARBANDPASS;

void initialize_VARBANDPASS(VARBANDPASS *vbf, float centerFreq, float Q, float sampleRate, uint8_t bandPass);
float update_VARBANDPASS(VARBANDPASS *vbf, float input);
void setCenterFrequency_VARBANDPASS(VARBANDPASS *vbf, float centerFreq, float Q);
void gui_setCenterFrequency_VARBANDPASS(VARBANDPASS *vbf);

#endif /* EFFECTS_COMPONENTS_VARIABLE_BANDPASS_FILTER_H_ */
