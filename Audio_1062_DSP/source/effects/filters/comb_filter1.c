/*
 * comb_filter1.c
 *
 *  Created on: May 13, 2022
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

#include <filters/comb_filter1.h>
#include <hardware_specific/denormals.h>

static void mute (COMB_FILTER *comb);

void initCombFilter1(COMB_FILTER * comb, float *buffer, int size, float gain) {
	comb->gain = gain;
	comb->buf = buffer;
	comb->bufLimit = size;
	comb->bufSize = size;
	comb->bufIndx = 0;
	mute(comb);
}
void upDateCombFilter1(COMB_FILTER * comb, float limitFactor) {
	comb->bufLimit = limitFactor;
	if (limitFactor >= comb->bufIndx) {
		comb->bufIndx = comb->bufLimit - 1;
	}
}
float applyCombFilter1(COMB_FILTER * comb, float input) {
	comb->out = comb->buf[comb->bufIndx];
	undenormalise(comb->out);

	comb->next = comb->out * comb->gain + input;
	comb->buf[comb->bufIndx] = comb->next;
	comb->bufIndx++;
	if (comb->bufIndx >= comb->bufLimit) comb->bufIndx = 0;
	return comb->out;
}
static void mute (COMB_FILTER *comb) {
	for(comb->i = 0; comb->i < comb->bufSize; comb->i++) {
		comb->buf[comb->i] = 0.0f;
	}
}

