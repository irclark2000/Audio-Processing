/*
 * equalizing_filter.c
 *
 *  Created on: Sep 11, 2024
 *      Author: isaac
 */
/*
 * Peak Equalizing filter formulas adapted from Robert Bristow-Johnson's Audio EQ Cookbook
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

#include <filters/equalizing_filter.h>
#include <math.h>

static const float PI = 3.14159265358979f;

void calculateCoefficients (EQFILTER *eqf);

void EQFILTER_initialize(EQFILTER *eqf, float centerFreq, float sampleRate, float gain, float bandwidth) {
	eqf->sampleTime = 1.0/sampleRate;
	eqf->gain = gain;
	EQFILTER_setCenterFrequency(eqf, centerFreq, bandwidth);
}

void calculateCoefficients (EQFILTER *eqf) {
	  // scale all coefficients by a0 and set a0 to 1.0
	//float A = powf (10.0f, eqf->gain/40.0f);
	//float Q = (eqf->inv_bandwidth)/eqf->sampleTime;
	float Q = eqf->Q;
	float K = tanf(eqf->eqfWct/2.0f);
	float inv_a0 = 1.0f/(1.0f + K/Q + K*K);
	eqf->coefficients.b0 =  (1.0f + eqf->gain*K/Q + K*K)*inv_a0;
	eqf->coefficients.b1 =  2.0f * (K*K - 1.0f)* inv_a0;
	eqf->coefficients.b2 =  (1.0f - eqf->gain*K/Q + K*K)*inv_a0;
	eqf->coefficients.a0 = 1.0f;
	eqf->coefficients.a1 = eqf->coefficients.b1;
	eqf->coefficients.a2 = (1.0f - K/Q + K*K)*inv_a0;

}

float EQFILTER_update(EQFILTER *eqf, float input) {
	eqf->eqfBufIn[2] = eqf->eqfBufIn[1];
	eqf->eqfBufIn[1] = eqf->eqfBufIn[0];
	eqf->eqfBufIn[0] = input;
	eqf->eqfBufOut[2] = eqf->eqfBufOut[1];
	eqf->eqfBufOut[1] = eqf->eqfBufOut[0];
	eqf->eqfBufOut[0] =
			eqf->coefficients.b0 * eqf->eqfBufIn[0] +
			eqf->coefficients.b1 * eqf->eqfBufIn[1] +
			eqf->coefficients.b2 * eqf->eqfBufIn[2] -
			eqf->coefficients.a1 * eqf->eqfBufOut[1] -
			eqf->coefficients.a2 * eqf->eqfBufOut[2];
	eqf->eqfOut = eqf->eqfBufOut[0];
	return eqf->eqfOut;
}

void EQFILTER_setGain(EQFILTER *eqf, float gain) {
	eqf->gain = gain;
	float Q = eqf->Q;
	float K = tanf(eqf->eqfWct/2.0f);
	float inv_a0 = 1.0f/(1.0f + K/Q + K*K);
	eqf->coefficients.b0 =  (1.0f + eqf->gain*K/Q + K*K)*inv_a0;
	eqf->coefficients.b2 =  (1.0f - eqf->gain*K/Q + K*K)*inv_a0;
	eqf->coefficients.a2 = (1.0f - K/Q + K*K)*inv_a0;

}
void EQFILTER_setCenterFrequency(EQFILTER *eqf, float centerFreq, float bandwidth) {
	eqf->eqfWct = 2.0f*PI*centerFreq * eqf->sampleTime;
	eqf->Q = centerFreq / bandwidth;
	calculateCoefficients(eqf);
}

void gui_EQFILTER_setCenterFrequency(EQFILTER *eqf) {
	EQFILTER_setCenterFrequency(eqf, eqf->gui_freq, eqf->gui_freq/eqf->Q);
}
