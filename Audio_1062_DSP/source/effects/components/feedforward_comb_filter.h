/*
 * feedforward_comb.filter.h
 *
 *  Created on: Oct 27, 2024
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

#ifndef EFFECTS_COMPONENTS_FEEDFORWARD_COMB_FILTER_H_
#define EFFECTS_COMPONENTS_FEEDFORWARD_COMB_FILTER_H_
#include <stdint.h>
#include <string.h>
#include "effects/components/effects_macros.h"

typedef struct {
	float sampleRate;
	float sampleTime;  // 1/sampleRate: avoids division in process loop
	float fcbfOut;
	float delayMSec;
	float b0;
	float bM;
	float *buf;
	uint32_t buf_size;
	uint32_t index;
} FFCF;

void initialize_FFCF (FFCF *filter, float *buf, uint32_t buf_size, float feedforwardGain, float directLevel, float sampleRate);
void setFeedforward_FFCF(FFCF *filter, float feedforward);
void setDirectLevel_FFCF(FFCF *filter, float direct);
float applyFilter_FFCF(FFCF *filter, float input);
void unInitialize_FFCF (FFCF *filter);


#endif /* EFFECTS_COMPONENTS_FEEDFORWARD_COMB_FILTER_H_ */
