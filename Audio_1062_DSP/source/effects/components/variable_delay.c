/*
 * variable_delay.c
 *
 *  Created on: Sep 21, 2024
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

#include <components/variable_delay.h>
#if AUDIO_EFFECTS_TESTER
#include <stdlib.h>
#endif

void initialize_variable_delay (VARDELAY * vDelay, float *buf, float buf_size, float sampleRate) {
#if AUDIO_EFFECTS_TESTER
	if (buf == 0) {
		buf = (float *) malloc(buf_size * sizeof(float));
	}
#endif
	vDelay->sampleTime = 1.0f / sampleRate;
	vDelay->sampleRate = sampleRate;
	vDelay->delayInSamples = 0;
	vDelay->max_delay = (buf_size - 1) * vDelay->sampleTime;
	vDelay->size = buf_size;
	vDelay->cBufPtr = &(vDelay->cBuf);
	cb_initialize(vDelay->cBufPtr, buf, buf_size);
}

/*
 *  gets a delayed sample and puts the input into the buffer.
 *  uses linear interpolation to fetch non integer delays
 *  consider using more advanced interpolation
 *  adds feedback (if any) to stored input
 */

float getDelayedSample_VARDELAY(VARDELAY *vDelay, float input, float fb_level) {
	// compute whole and fractional indices
	float offset = (vDelay->delayInSamples);
	float f_index = vDelay->cBuf.wr_ptr - offset;
	// don't allow negative values for f_index - floating point modulo arithmetic
	while (f_index < 0.0f) f_index += vDelay->size;

	// compute whole and fractional parts of index
	uint32_t whole_index = (uint32_t) f_index;
	float frac_index = f_index - whole_index;

	vDelay->out = getFloatAtIndex_VARDELAY(vDelay, whole_index);

	// implements fractional part of delay using linear interpolation
	if (frac_index >= 0.01 && offset > 1) {
		float value1 = getFloatAtIndex_VARDELAY(vDelay, whole_index + 1);
		vDelay->out = value1 * frac_index + vDelay->out * (1.0f - frac_index);
	}
	// save the input value + fb-level fraction of output (regenerative feedback)
	float save_value = input + vDelay->out * fb_level;
	vDelay->cBuf.storage[vDelay->cBuf.wr_ptr] = save_value;

	// increment write pointer
	vDelay->cBuf.wr_ptr = (vDelay->cBuf.wr_ptr + 1) % vDelay->size;
	// increments read pointer, but we are not using this
	vDelay->cBuf.rd_ptr = (vDelay->cBuf.rd_ptr + 1) % vDelay->size;
	return vDelay->out;
}
void setDelay_VARDELAY(VARDELAY *vDelay, float delaySec) {
	float seconds = (delaySec <= vDelay->max_delay) ? (delaySec >= vDelay->sampleTime ? delaySec : vDelay->sampleTime) : vDelay->max_delay;
	vDelay->delayInSamples = vDelay->sampleRate * seconds;
	int32_t ptr = (int32_t) (vDelay->cBuf.wr_ptr) - vDelay->delayInSamples;
	setReadPointer(vDelay, ptr); // not currently being used
}

void setReadPointer(VARDELAY *vDelay, int32_t value) {
  while (value < 0) {
	  value += vDelay->size;
  }
  value %= vDelay->size;
  vDelay->cBufPtr->rd_ptr = value;
}
float getFloatAtIndex_VARDELAY (VARDELAY *vDelay, int32_t index) {
	//CIRCBUFFER *cb = &(vDelay->cBuf);
	index = (index + 5 * vDelay->cBufPtr->size) % vDelay->cBufPtr->size;
	return vDelay->cBufPtr->storage[index];
}
float getFloatAtReadPtrWithIndex_VARDELAY(VARDELAY *vDelay, uint32_t index) {
	index = (index + 5 * vDelay->cBufPtr->size) % vDelay->cBufPtr->size;
	return vDelay->cBufPtr->storage[index];
}
#if AUDIO_EFFECTS_TESTER
void uninitialize_VARDELAY(VARDELAY *vDelay) {
	free(vDelay->cBuf);
}
#endif
