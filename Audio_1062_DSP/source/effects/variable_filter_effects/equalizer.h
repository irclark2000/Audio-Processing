/*
 * equalizer.h
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

#ifndef EFFECTS_VARIABLE_FILTER_EFFECTS_EQUALIZER_H_
#define EFFECTS_VARIABLE_FILTER_EFFECTS_EQUALIZER_H_
#include "effects/filters/equalizing_filter.h"
#include <stdint.h>

#define EQUALIZER_BAND_COUNT 10
typedef struct {
	uint8_t filterCount;
	float inv_Count;
	int parallel;
	EQFILTER filter_band[EQUALIZER_BAND_COUNT];
	float out;
} EQUALIZER;

void initialize_EQUALIZER(EQUALIZER *eq, float sampleRate);
float apply_EQUALIZER(EQUALIZER *eq, float input);

#endif /* EFFECTS_VARIABLE_FILTER_EFFECTS_EQUALIZER_H_ */
