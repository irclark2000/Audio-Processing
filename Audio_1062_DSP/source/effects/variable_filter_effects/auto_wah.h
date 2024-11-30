/*
 * auto_wah.h
 *
 *  Created on: Nov 1, 2024
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

#ifndef EFFECTS_VARIABLE_FILTER_EFFECTS_AUTO_WAH_H_
#define EFFECTS_VARIABLE_FILTER_EFFECTS_AUTO_WAH_H_

#include "effects/components/envelope_follower.h"
#include "effects/components/variable_bandpass_filter.h"
#include "effects/components/mixer.h"
#include "effects/components/effects_macros.h"

typedef struct {
	float awOut;
	float sampleRate;
	ENVELOPE_FOLLOWER ef;
	VARBANDPASS vbf;
	float inputGain;
	float fxGain;
	int down_scan;
	float minCoFreq;
	float maxCoFreq;
} AUTOWAH;

void initialize_AUTOWAH (AUTOWAH *aw, float inputGain, float outputGain, float minCoFreq, float maxCoFreq, float sampleRate);
float apply_AUTOWAH (AUTOWAH *aw, float input);
void setFxGain_AUTOWAH (AUTOWAH *aw, float outputGain);
void setInputGain_AUTOWAH (AUTOWAH *aw, float inputGain);

#endif /* EFFECTS_VARIABLE_FILTER_EFFECTS_AUTO_WAH_H_ */
