/*
 * auto_wah.c
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

#include "auto_wah.h"
#include "components/memory_debug.h"
void initialize_MIXER (MIXER * mixer, float wet_dry);
float applyWetDry_MIXER (MIXER * mixer, float input_wet, float input_dry);
void setWetDry_MIXER (MIXER *mixer, float wet_dry);

void initialize_VARBANDPASS(VARBANDPASS *vbf, float centerFreq, float Q, float sampleRate, uint8_t bandPass);
float update_VARBANDPASS(VARBANDPASS *vbf, float input);
void setCenterFrequency_VARBANDPASS(VARBANDPASS *vbf, float centerFreq, float Q);

void initialize_ENVELOPE_FOLLOWER(ENVELOPE_FOLLOWER *ef, float sampleRate);
float apply_ENVELOPE_FOLLOWER(ENVELOPE_FOLLOWER *ef, float input);

static float coFreq;

void initialize_AUTOWAH (AUTOWAH *aw, float inputGain, float fxGain, float minCoFreq, float maxCoFreq, float sampleRate) {
	aw->inputGain = 1.0f;
	aw->fxGain = 1.0f;
	aw->minCoFreq = minCoFreq;
	aw->maxCoFreq = maxCoFreq;

	aw->sampleRate = sampleRate;
	setFxGain_AUTOWAH (aw, fxGain);
	setInputGain_AUTOWAH (aw, inputGain);
	initialize_ENVELOPE_FOLLOWER(&(aw->ef), sampleRate);
	initialize_MIXER (&(aw->mixer), 0.5f);
	initialize_VARBANDPASS(&(aw->vbf), 1000.0f, 8, sampleRate, 1);
}
float apply_AUTOWAH (AUTOWAH *aw, float input) {
	apply_ENVELOPE_FOLLOWER(&(aw->ef), input * aw->inputGain);
	coFreq = aw->minCoFreq + (aw->maxCoFreq - aw->minCoFreq) * aw->ef.efOut;
#if AUDIO_EFFECTS_TESTER
	setCenterFrequency_VARBANDPASS(&(aw->vbf), aw->vbf.gui_Freq, aw->vbf.gui_Q);
#else
	setCenterFrequency_VARBANDPASS(&(aw->vbf), coFreq, 3.0);
#endif
	update_VARBANDPASS(&(aw->vbf), input);
	aw->awOut = applyWetDry_MIXER (&(aw->mixer), aw->vbf.out * aw->fxGain, input);
	return aw->awOut;
}
void setFxGain_AUTOWAH (AUTOWAH *aw, float fxGain) {
	fxGain = MIN_MAX(fxGain, 0.0f, 2.0f);
	aw->fxGain = fxGain;
}
void setInputGain_AUTOWAH (AUTOWAH *aw, float inputGain) {
	inputGain = MIN_MAX(inputGain, 0.0f, 2.0f);
	aw->inputGain = inputGain;
}
void setWetDry_AUTOWAH (AUTOWAH *aw, float wet_dry) {
	setWetDry_MIXER (&(aw->mixer), wet_dry);
}
