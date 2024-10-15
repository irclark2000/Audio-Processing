/*
 * chorus.h
 *
 *  Created on: Sep 24, 2024
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

#ifndef EFFECTS_DELAY_BASED_CHORUS_H_
#define EFFECTS_DELAY_BASED_CHORUS_H_

#include <components/mixer.h>
#include "chorus_element.h"

#define MAX_NUMBER_CHORUSELEMENTS 4
typedef struct {
	CHORUSELEMENT cElement[MAX_NUMBER_CHORUSELEMENTS];
	uint8_t chorus_count;
	float inv_count;   // allows using multiplication rather than division
	float sampleRate;
	float output;
	MIXER mixer;
} CHORUS;

void initialize_CHORUS (CHORUS *chorus, uint8_t chorus_count, float *buf[], float wet_dry,
		uint32_t buf_size, float delayMSec, CELEMENTDATA *cData, float sampleRate);
void setWetDry_CHORUS (CHORUS *chorus, float wet_dry);
void setBaseDelayMSec_CHORUS (CHORUS *chorus, float delayMS);
void setDepth_CHORUS(CHORUS *chorus, float depth, uint8_t element);
void setFrequency_CHORUS(CHORUS *chorus, float freq, uint8_t element);
float update_CHORUS (CHORUS *chorus, float input);


#endif /* EFFECTS_DELAY_BASED_CHORUS_H_ */
