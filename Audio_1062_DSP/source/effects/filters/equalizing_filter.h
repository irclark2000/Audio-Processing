/*
 * equalizing filter.h
 *
 *  Created on: Sep 11, 2024
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

#ifndef EQUALIZING_FILTER_H_
#define EQUALIZING_FILTER_H_

#include <stdint.h>
#include "filter_coefficients.h"

typedef struct {

	float sampleTime;
	float eqfBufIn[3];
	float eqfBufOut[3];
	float eqfWct;
	float eqfOut;

	float gain;
	float Q;
	float gui_gainDB;
	float gui_freq;

	FILTER_COEF coefficients;

} EQFILTER;

void initialize_EQFILTER(EQFILTER *eqf, float centerFreq, float sampleRate, float gain, float bandwidth);
float apply_EQFILTER(EQFILTER *eqf, float input);
void EQFILTER_setGain(EQFILTER *eqf, float gain);
void setCenterFrequency_EQFILTER(EQFILTER *eqf, float centerFreq, float bandwidth);
void gui_setGain_EQFILTER(EQFILTER *eqf);
#endif /* EQUALIZING_FILTER_H_ */
