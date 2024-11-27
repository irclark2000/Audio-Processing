/*
 * equalizer.c
 *
 *  Created on: Nov 14, 2024
 *      Author: isaac
 */
/*
Copyright 2022 Isaac R. Clark, Jr.

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
#include "equalizer.h"

typedef struct {
	float frequency;
	float Q;
} EQPARAMS;

EQPARAMS bands[] = { {31.25f, 4}, {62.5f, 4}, {125, 4}, {250, 4},
		{500, 4}, {1000, 4}, {2000, 4}, {4000, 4},
		{8000, 4}, {16000, 4}
};
void initialize_EQUALIZER(EQUALIZER *eq, float sampleRate) {
	int myCount = sizeof(bands) / sizeof(EQPARAMS);
	eq->filterCount = myCount;
	eq->inv_Count  = 1.0f/myCount;
	for (int i=0; i < myCount; ++i) {
		initialize_EQFILTER(&(eq->filter_band[i]), bands[i].frequency, sampleRate, 1.0, bands[i].frequency/bands[i].Q);
	}
	eq->parallel = 1;
}

float apply_EQUALIZER(EQUALIZER *eq, float input) {
	eq->out = (eq->parallel == 0) ? input : 0.0f;
	for (int i=0; i < eq->filterCount; ++i) {
		if (eq->parallel == 0) {
			eq->out = apply_EQFILTER(&(eq->filter_band[i]), eq->out);
		}
		else {
			eq->out += apply_EQFILTER(&(eq->filter_band[i]), input) * eq->inv_Count ;
		}
	}
	return eq->out;
}
