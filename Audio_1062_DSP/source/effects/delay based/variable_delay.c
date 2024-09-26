/*
 * variable_delay.c
 *
 *  Created on: Sep 21, 2024
 *      Author: isaac
 */

#include "variable_delay.h"

void initialize_variable_delay (VARDELAY * vDelay, float *buf, float buf_size, float sampleRate) {
	vDelay->sampleTime = 1.0f / sampleRate;
	vDelay->sampleRate = sampleRate;
	vDelay->delayInSamples = 0;
	vDelay->max_delay = (buf_size - 1) * vDelay->sampleTime;
	vDelay->size = buf_size;
	cb_initialize(&(vDelay->cBuf), buf, buf_size);
}

/*
 *  gets a delayed sample and puts the input into the buffer.
 *  adds feedback if any
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

	vDelay->out = cb_getFloatAtIndex(&(vDelay->cBuf), whole_index);

	// implements fractional part of delay using linear interpolation
	if (frac_index >= 0.01 && offset > 1) {
		float value1 = cb_getFloatAtIndex(&(vDelay->cBuf), whole_index + 1);
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
	setReadPointer(vDelay, ptr);
}

void setReadPointer(VARDELAY *vDelay, int32_t value) {
  while (value < 0) {
	  value += vDelay->size;
  }
  value %= vDelay->size;
  vDelay->cBuf.rd_ptr = value;
}

