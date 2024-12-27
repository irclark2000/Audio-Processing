/*
 * first_order_lowpass.h
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

#ifndef EFFECTS_FILTERS_FIRST_ORDER_LOWPASS_H_
#define EFFECTS_FILTERS_FIRST_ORDER_LOWPASS_H_
typedef struct FIRSTLOWPASSFILTER {
	float sampleRate;
	float sampleTime;
	float alpha;
	float lpfOut;
}FIRSTLOWPASSFILTER;

void initialize_FIRSTLOWPASSFILTER(FIRSTLOWPASSFILTER *lpf, float centerFreq, float sampleRate);
float apply_FIRSTLOWPASSFILTER(FIRSTLOWPASSFILTER *lpf, float input);
void setCenterFrequency_FIRSTLOWPASSFILTER(FIRSTLOWPASSFILTER *lpf, float centerFreq);

#endif /* EFFECTS_FILTERS_FIRST_ORDER_LOWPASS_H_ */
