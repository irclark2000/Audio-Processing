/*
 * second_order_all_pass.h
 *
 *  Created on: Oct 1, 2024
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


#ifndef EFFECTS_FILTERS_SECOND_ORDER_ALL_PASS_H_
#define EFFECTS_FILTERS_SECOND_ORDER_ALL_PASS_H_

#include <stdint.h>
#include "filter_coefficients.h"

typedef struct {
	float sampleRate;
	float sampleTime;
	float bufIn[3];
	float bufOut[3];
	float apfWct;
	float apfOut;
	float tanF;
	float gui_Q;
	float gui_Freq;

	float inv_Q;

	FILTER_COEF coefficients;

} SECONDALLPASSFILTER;

void initialize_SECONDALLPASSFILTER(SECONDALLPASSFILTER *apf, float centerFreq, float Q, float sampleRate);
float update_SECONDALLPASSFILTER(SECONDALLPASSFILTER *apf, float input);
void setCenterFrequency_SECONDALLPASSFILTER(SECONDALLPASSFILTER *apf, float centerFreq, float Q);


#endif /* EFFECTS_FILTERS_SECOND_ORDER_ALL_PASS_H_ */
