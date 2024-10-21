/*
 * comb_filter.c
 *
 *  Created on: Apr 8, 2022
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

#include <filters/comb_filter.h>
#include <hardware_specific/denormals.h>
#if AUDIO_EFFECTS_TESTER
#include <stdlib.h>
#endif

static void mute(LPFCOMB_FILTER *comb);

void initCombFilter(LPFCOMB_FILTER *comb, float *buffer, int size, float damp,
		float feedback) {
#if AUDIO_EFFECTS_TESTER
	if (buffer == 0) {
		buffer = (float *) malloc(int * sizeof(float));
	}
#endif
	comb->damp = damp;
	comb->non_damp = 1.0f - damp;
	comb->buf = buffer;
	comb->bufLimit = size;
	comb->bufSize = size;
	comb->bufIndx = 0;
	comb->filterstore = 0;
	comb->feedback = feedback;
	mute(comb);

}
float applyCombFilter(LPFCOMB_FILTER *comb, float input) {

	comb->out = comb->buf[comb->bufIndx];
	if (comb->out != comb->out) {
		comb->i = 10;
	}
	undenormalise(comb->out);

	comb->filterstore = (comb->out * comb->non_damp)
			+ (comb->filterstore * comb->damp);
	undenormalise(comb->filterstore);

	comb->buf[comb->bufIndx] = input + (comb->filterstore * comb->feedback);
	if (++(comb->bufIndx) >= comb->bufLimit)
		comb->bufIndx = 0;

	return comb->out;
}

static void mute(LPFCOMB_FILTER *comb) {
	for (comb->i = 0; comb->i < comb->bufSize; comb->i++) {
		comb->buf[comb->i] = 0.0f;
	}
}

