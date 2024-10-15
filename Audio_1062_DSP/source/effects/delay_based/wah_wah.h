/*
 * wah_wah.h
 *
 *  Created on: Oct 3, 2024
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

#ifndef EFFECTS_DELAY_BASED_WAH_WAH_H_
#define EFFECTS_DELAY_BASED_WAH_WAH_H_

#include "components/mixer.h"
#include "components/components.h"
#include "components/low_frequency_oscillator.h"
#include "components/state_variable_filter.h"

typedef struct {
	LOWFREQOSC lfo;
	SVFILTER svf;
	MIXER mixer;
	float out;
	float wahFreq;
	PARAMETER_LIMITS cutoff_limits;
}WAHWAH;
void initialize_WAHWAH(WAHWAH * ww, float wahFreq, float minCutOffFreq, float maxCutoffFreq,
		float damp, float wet_dry, float sampleRate);
float apply_WAHWAH (WAHWAH * ww, float input);
void setMInMAXCutoffFrequency (WAHWAH *ww, float minCutoffFreq, float maxCutoffFreq);
void setWahFreq_Damping_WAHWAH(WAHWAH * ww, float wahFreq, float damp);

#endif /* EFFECTS_DELAY_BASED_WAH_WAH_H_ */
