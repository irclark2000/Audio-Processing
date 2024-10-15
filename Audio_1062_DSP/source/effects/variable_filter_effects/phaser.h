/*
 * phaser.h
 *
 *  Created on: Oct 7, 2024
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

#ifndef EFFECTS_VARIABLE_FILTER_EFFECTS_PHASER_H_
#define EFFECTS_VARIABLE_FILTER_EFFECTS_PHASER_H_

#include "components/first_order_allpass.h"
#include "components/low_frequency_oscillator.h"
#include "components/mixer.h"

#define MAX_NUMBER_ALLPASS 4

typedef struct {
	FIRSTORDERALLPASSFILTER apf[MAX_NUMBER_ALLPASS];
	LOWFREQOSC lfo;
	MIXER mixer;
	uint8_t index;
	float phaserOut;
	float baseFrequency;
	float yPrev;
	uint8_t filterCount;
	float feedbackGain;
} PHASER;

void initialize_PHASER(PHASER * phaser, uint8_t filterCount, float breakFrequency, float minBreak, float maxBreak,
		float sweepFrequency, float minSweep, float maxSweep, float feedbackGain, float sampleRate);
float update_PHASER(PHASER *phaser, float input);
void setBreakFrequency_PHASER(PHASER *phaser, float centerFreq);
void setWetDry_PHASER (PHASER *phaser, float wet_dry);
void setFeedbackGain_PHASER (PHASER *phaser, float fb_gain);

#endif /* EFFECTS_VARIABLE_FILTER_EFFECTS_PHASER_H_ */
