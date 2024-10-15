/*
 * phaser.c
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


#include <variable_filter_effects/phaser.h>
#include "components/effects_macros.h"
void initialize_PHASER(PHASER * phaser, uint8_t filterCount, float breakFrequency, float minBreak, float maxBreak,
		float sweepFrequency, float minSweep, float maxSweep, float feedbackGain, float sampleRate) {
	phaser->filterCount = MIN_MAX(filterCount, 2, MAX_NUMBER_ALLPASS);
	phaser->yPrev = 0;
	setFeedbackGain_PHASER(phaser, feedbackGain);

	phaser->baseFrequency = sweepFrequency;
	for (phaser->index = 0; phaser->index < phaser->filterCount; phaser->index++) {
		initialize_FIRSTORDERALLPASSFILTER(&(phaser->apf[phaser->index]), breakFrequency, sampleRate);
	}
	// FIXME;  Check Depth parameters
	initialize_LOWFREQOSC (&(phaser->lfo),
			(minBreak - minBreak), (minBreak+ maxBreak)/ 2.0f,
			maxBreak,
			sweepFrequency, minSweep, maxSweep,
			0.0f,
			1, // sinusoidal output
			sampleRate);
	initialize_MIXER (&(phaser->mixer), 0.5f);
}

float update_PHASER(PHASER *phaser, float input) {
	update_LOWFREQOSC(&(phaser->lfo));
	setBreakFrequency_PHASER(phaser, phaser->baseFrequency + phaser->lfo.out);
	// add the feedback to the input
	phaser->phaserOut = input + phaser->yPrev * phaser->feedbackGain;
	// for loop applies the output each allpass to the following stage, if any
	for (phaser->index = 0; phaser->index < phaser->filterCount; phaser->index++) {
		phaser->phaserOut = update_FIRSTORDERALLPASSFILTER(&(phaser->apf[phaser->index]),
				phaser->phaserOut);
	}
	phaser->yPrev = phaser->phaserOut;   // store all filter output for next iteration
	phaser->phaserOut = applyWetDry_MIXER (&(phaser->mixer), phaser->phaserOut, input);
	return phaser->phaserOut;
}

void setBreakFrequency_PHASER(PHASER *phaser, float breakFreq) {
	for (phaser->index = 0; phaser->index < phaser->filterCount; phaser->index++) {
		setCenterFrequency_FIRSTORDERALLPASSFILTER(&(phaser->apf[phaser->index]),
				breakFreq);
	}
}
void setWetDry_PHASER (PHASER *phaser, float wet_dry) {
	setWetDry_MIXER (&(phaser->mixer), wet_dry);
}
void setFeedbackGain_PHASER (PHASER *phaser, float fb_gain) {
	phaser->feedbackGain = MIN_MAX(fb_gain, 0, 0.98);
}

void setFreq_LOWFREQOSC(LOWFREQOSC *osc, float osc_freq);
void setAmplitude_LOWFREQOSC(LOWFREQOSC *osc, float amplitude);
