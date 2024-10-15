/*
 * chorus_element.h
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

#ifndef EFFECTS_DELAY_BASED_CHORUS_ELEMENT_H_
#define EFFECTS_DELAY_BASED_CHORUS_ELEMENT_H_
#include <components/low_frequency_oscillator.h>
#include <components/variable_delay.h>
#include <stdint.h>

typedef struct {
	LOWFREQOSC lfo;
	VARDELAY vDelay;
	float baseDelayMSec;
	float output;
} CHORUSELEMENT;

typedef struct {
	float freq;
	float depth;
} CELEMENTDATA;

void initialize_CHORUSELEMENT(CHORUSELEMENT * cEl, float * buf, uint32_t buf_size, float delayMSec,
		CELEMENTDATA cdata, float sampleRate);
void setBaseDelayMSec_CHORUSELEMENT(CHORUSELEMENT * cEl, float delayMSec);
void setDepth_CHORUSELEMENT(CHORUSELEMENT * cEl, float depth);
void setFrequency_CHORUSELEMENT(CHORUSELEMENT * cEl, float freq);
float update_CHORUSELEMENT (CHORUSELEMENT * cEl, float input);
#endif /* EFFECTS_DELAY_BASED_CHORUS_ELEMENT_H_ */
