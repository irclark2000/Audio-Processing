/*
 * second_order_low_pass.c
 *
 *  Created on: Nov 13, 2024
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

#include <math.h>
#include "second_order_low_pass.h"
#include "components/effects_macros.h"

static const float PI = 3.14159265358979f;

void calculateCoefficients_SECONDLOWPASSFILTER (SECONDLOWPASSFILTER *lpf);

void initialize_SECONDLOWPASSFILTER(SECONDLOWPASSFILTER *lpf, float centerFreq,
		float Q, float sampleRate) {
	lpf->sampleTime = 1.0/sampleRate;
	lpf->sampleRate = sampleRate;
	setCutoffFrequency_SECONDLOWPASSFILTER(lpf, centerFreq, Q);
	lpf->bufIn[2] = lpf->bufIn[1] = lpf->bufIn[0] = 0.0f;
	lpf->bufOut[2] = lpf->bufOut[1] = lpf->bufOut[0] = 0.0f;
}

void calculateCoefficients_SECONDLOWPASSFILTER (SECONDLOWPASSFILTER *lpf) {
	lpf->norm = 1.0f / (1.0f + lpf->K*lpf->inv_Q + lpf->K * lpf->K);
	lpf->coefficients.b0 =  lpf->K * lpf->K * lpf->norm;
	lpf->coefficients.b1 =  2.0f * lpf->coefficients.b0;
	lpf->coefficients.b2 =  lpf->coefficients.b0;
	lpf->coefficients.a0 = 1.0f;
	lpf->coefficients.a1 = 2.0f * (lpf->K * lpf->K - 1.0f) * lpf->norm;
	lpf->coefficients.a2 = (1.0f - lpf->K *lpf->inv_Q + lpf->K * lpf->K) * lpf->norm;
}

float apply_SECONDLOWPASSFILTER(SECONDLOWPASSFILTER *lpf, float input) {
	lpf->bufIn[2] = lpf->bufIn[1];
	lpf->bufIn[1] = lpf->bufIn[0];
	lpf->bufIn[0] = input;
	lpf->bufOut[2] = lpf->bufOut[1];
	lpf->bufOut[1] = lpf->bufOut[0];
	lpf->bufOut[0] =
			lpf->coefficients.b0 * lpf->bufIn[0] +
			lpf->coefficients.b1 * lpf->bufIn[1] +
			lpf->coefficients.b2 * lpf->bufIn[2] -
			lpf->coefficients.a1 * lpf->bufOut[1] -
			lpf->coefficients.a2 * lpf->bufOut[2];
	lpf->lpfOut = lpf->bufOut[0];
	return lpf->lpfOut;
}

void setCutoffFrequency_SECONDLOWPASSFILTER(SECONDLOWPASSFILTER *lpf, float centerFreq, float Q) {
	centerFreq = MIN_MAX (centerFreq, 0.01f, 0.5 * lpf->sampleRate);
	lpf->lpfWct = 2.0f*PI*centerFreq * lpf->sampleTime;
	lpf->inv_Q = 1.0 / Q;
	lpf->K = tanf(0.5 * lpf->lpfWct * lpf->inv_Q);
	calculateCoefficients_SECONDLOWPASSFILTER(lpf);
}

void gui_setCutoffFrequency_SECONDLOWPASSFILTER(SECONDLOWPASSFILTER *lpf) {
	setCutoffFrequency_SECONDLOWPASSFILTER(lpf, lpf->gui_Freq, lpf->Q);
}
