/*
 * vibrato.c
 *
 *  Created on: Sep 30, 2024
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


#include "vibrato.h"
#include <math.h>
#include "components/effects_macros.h"

void initialize_VIBRATO (VIBRATO * vb, float *buf, float buf_size,
		float base_vibrato_frequency, float delayMSec, float variation_delayMSec, float sampleRate) {
	vb->delay_limitsMsec.minimum = 1000.0f / sampleRate;
	vb->delay_limitsMsec.maximum = buf_size * 1000.0f / sampleRate;
	vb->baseDelayMSec = delayMSec;
  initialize_LOWFREQOSC (&(vb->lfo), variation_delayMSec, 0.1, 100, base_vibrato_frequency, 0.01f, 100.0f, 0.0f, sampleRate);
  initialize_MIXER (&(vb->mixer), 0.5);
  initialize_variable_delay (&(vb->vDelay), buf, buf_size, sampleRate);
}

float apply_VIBRATO (VIBRATO * vb, float input) {
	update_LOWFREQOSC(&(vb->lfo));
	setDelay_VARDELAY(&(vb->vDelay), (vb->baseDelayMSec + fabs(vb->lfo.out))/1000.0f);
	vb->out = getDelayedSample_VARDELAY(&(vb->vDelay), input, 0.0f);
	vb->out = applyWetDry_MIXER (&(vb->mixer), vb->out, input);
	return vb->out;
}
void setWetDry_VIBRATO (VIBRATO *vb, float wet_dry) {
	setWetDry_MIXER (&(vb->mixer), wet_dry);
}
void setBaseDelayMSec_VIBRATO (VIBRATO * vb, float delayMSec) {
	delayMSec = MIN_MAX(delayMSec, vb->delay_limitsMsec.minimum, vb->delay_limitsMsec.maximum);
	vb->baseDelayMSec = delayMSec;
}

void setFreq_LOWFREQOSC(LOWFREQOSC *osc, float osc_freq);
void setAmplitude_LOWFREQOSC(LOWFREQOSC *osc, float amplitude);
void update_LOWFREQOSC(LOWFREQOSC *osc);

void setFreq_VIBRATO(VIBRATO * vb, float osc_freq) {
	void setFreq_LOWFREQOSC(LOWFREQOSC *osc, float osc_freq);
}
void setAmplitude_VIBRATO(VIBRATO * vb, float amplitude) {
	setAmplitude_LOWFREQOSC(&(vb->lfo), amplitude);

}
