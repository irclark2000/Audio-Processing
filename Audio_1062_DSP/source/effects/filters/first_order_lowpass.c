/*
 * first_order_lowpass.c
 *
 *  Created on: Dec 21, 2024
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

#include "first_order_lowpass.h"
static const float PI = 3.14159265358979f;

void initialize_FIRSTLOWPASSFILTER(FIRSTLOWPASSFILTER *lpf, float centerFreq, float sampleRate) {
	lpf->sampleRate = sampleRate;
	lpf->sampleTime = 1.0f / sampleRate;
	lpf->lpfOut = 0.0f;
	setCenterFrequency_FIRSTLOWPASSFILTER(lpf, centerFreq);
}
float apply_FIRSTLOWPASSFILTER(FIRSTLOWPASSFILTER *lpf, float input) {
	lpf->lpfOut = (1.0f - lpf->alpha) * input + lpf->alpha * lpf->lpfOut;
	return lpf->lpfOut;
}
void setCenterFrequency_FIRSTLOWPASSFILTER(FIRSTLOWPASSFILTER *lpf, float centerFreq) {
	lpf->alpha = 1.0f / (1.0f + 2.0f * PI * centerFreq * lpf->sampleTime);
}

