/*
 * second_order_all_pass.c
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

#include <math.h>
#include "second_order_all_pass.h"
#include "components/effects_macros.h"

static const float PI = 3.14159265358979f;

void calculateCoefficients_SECONDALLPASSFILTER (SECONDALLPASSFILTER *apf);

void initialize_SECONDALLPASSFILTER(SECONDALLPASSFILTER *apf, float centerFreq,
		float Q, float sampleRate) {
	apf->sampleTime = 1.0/sampleRate;
	apf->sampleRate = sampleRate;
	setCenterFrequency_SECONDALLPASSFILTER(apf, centerFreq, Q);
	apf->bufIn[2] = apf->bufIn[1] = apf->bufIn[0] = 0.0f;
	apf->bufOut[2] = apf->bufOut[1] = apf->bufOut[0] = 0.0f;
}

void calculateCoefficients_SECONDALLPASSFILTER (SECONDALLPASSFILTER *apf) {
	/*
	 *  all pass filter coefficient calculation (python impl)
		tan = np.tan(np.pi * BW / fs)
		c = (tan - 1) / (tan + 1)
	    d = - np.cos(2 * np.pi * break_frequency / fs)

	    b = [-c, d * (1 - c), 1]
	    a = [1, d * (1 - c), -c]
	*/
	// verified coefficients by plotting filters using octave.
	float c = (apf->tanF - 1.0f) / (apf->tanF + 1.0f);
	float d = -cosf(apf->apfWct);
	// temporarily reverse a and b
	// bu we then put them back
	apf->coefficients.b0 =  -c;
	apf->coefficients.b1 =  d * (1.0f - c);
	apf->coefficients.b2 =  1.0f;
	apf->coefficients.a0 = 1.0f;
	apf->coefficients.a1 = apf->coefficients.b1;
	apf->coefficients.a2 = -c;
}

float update_SECONDALLPASSFILTER(SECONDALLPASSFILTER *apf, float input) {
	apf->bufIn[2] = apf->bufIn[1];
	apf->bufIn[1] = apf->bufIn[0];
	apf->bufIn[0] = input;
	apf->bufOut[2] = apf->bufOut[1];
	apf->bufOut[1] = apf->bufOut[0];
	apf->bufOut[0] =
			apf->coefficients.b0 * apf->bufIn[0] +
			apf->coefficients.b1 * apf->bufIn[1] +
			apf->coefficients.b2 * apf->bufIn[2] -
			apf->coefficients.a1 * apf->bufOut[1] -
			apf->coefficients.a2 * apf->bufOut[2];
	apf->apfOut = apf->bufOut[0];
	return apf->apfOut;
}

void setCenterFrequency_SECONDALLPASSFILTER(SECONDALLPASSFILTER *apf, float centerFreq, float Q) {
	centerFreq = MIN_MAX (centerFreq, 0.01f, 0.5 * apf->sampleRate);
	apf->apfWct = 2.0f*PI*centerFreq * apf->sampleTime;
	apf->inv_Q = 1.0 / Q;
	apf->tanF = tanf(0.5 * apf->apfWct * apf->inv_Q);
	calculateCoefficients_SECONDALLPASSFILTER(apf);
}
