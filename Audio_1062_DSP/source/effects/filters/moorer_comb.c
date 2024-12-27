/*
 * moorer_comb.c
 *
 *  Created on: Dec 21, 2024
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

#include "moorer_comb.h"
#include <stdlib.h>
#include <hardware_specific/denormals.h>


static void mute (MOORER_COMB *comb);

void initialize_MOORER_COMB(MOORER_COMB * comb, float *buffer, uint32_t size, float lpf_frequency, float gain, float sampleRate) {
comb->use_free = 0;
#if AUDIO_EFFECTS_TESTER
	if (buffer == 0) {
		buffer = (float *) malloc(size * sizeof(float));
		comb->use_free = 1;
	}
#endif
	comb->gain = gain;
	comb->buf = buffer;
	comb->bufLimit = size;
	comb->bufSize = size;
	comb->bufIndx = 0;
	mute(comb);
	initialize_FIRSTLOWPASSFILTER(&(comb->lowpass), lpf_frequency, sampleRate);

}
float apply_MOORER_COMB(MOORER_COMB * comb, float input) {
	comb->out = comb->buf[comb->bufIndx];
	undenormalise(comb->out);

	comb->next = apply_FIRSTLOWPASSFILTER(&(comb->lowpass), comb->out * comb->gain) + input;
	comb->buf[comb->bufIndx] = comb->next;
	comb->bufIndx++;
	if (comb->bufIndx >= comb->bufLimit) comb->bufIndx = 0;
	return comb->out;

}
void uninitialize_MOORER_COMB(MOORER_COMB *comb) {
	if (comb->use_free != 0) {
		free(comb->buf);
	}
}

static void mute (MOORER_COMB *comb) {
	for(comb->i = 0; comb->i < comb->bufSize; comb->i++) {
		comb->buf[comb->i] = 0.0f;
	}
}
