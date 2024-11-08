/*
 * first_order_allpass.h
 *
 *  Created on: Oct 7, 2024
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
#ifndef EFFECTS_COMPONENTS_FIRST_ORDER_ALLPASS_H_
#define EFFECTS_COMPONENTS_FIRST_ORDER_ALLPASS_H_

#include <stdint.h>
#include "filters/filter_coefficients.h"

typedef struct {
	float sampleRate;
	float sampleTime;  // 1/sampleRate: avoids division in process loop
	float apfOut;
	float tanF;        // avoid creating local variable
	float delay_n_1;
	float gui_centerFreq;

	float a1;

} FIRSTORDERALLPASSFILTER;

void initialize_FIRSTORDERALLPASSFILTER(FIRSTORDERALLPASSFILTER *apf, float centerFreq, float sampleRate);
float update_FIRSTORDERALLPASSFILTER(FIRSTORDERALLPASSFILTER *apf, float input);
void setCenterFrequency_FIRSTORDERALLPASSFILTER(FIRSTORDERALLPASSFILTER *apf, float centerFreq);
void gui_setCenterFrequency_FIRSTORDERALLPASSFILTER(FIRSTORDERALLPASSFILTER *apf);

#endif /* EFFECTS_COMPONENTS_FIRST_ORDER_ALLPASS_H_ */
