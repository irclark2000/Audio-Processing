/*
 * variable_delay.h
 *
 *  Created on: Sep 21, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_DELAY_BASED_VARIABLE_DELAY_H_
#define EFFECTS_DELAY_BASED_VARIABLE_DELAY_H_

#include <stdint.h>
#include "circular_buffer.h"
typedef struct {
	float max_delay;  // seconds
	float delayInSamples;
	float sampleTime;
	float sampleRate;
	float out;
	uint32_t size;
	CIRCBUFFER cBuf;
}VARDELAY;

void initialize_variable_delay (VARDELAY * vDelay, float *buf, float buf_size, float sampleRate);
float getDelayedSample_VARDELAY(VARDELAY *vDelay, float input, float fb_level);
void setDelay_VARDELAY(VARDELAY *vDelay, float delaySec);
void setReadPointer(VARDELAY *vDelay, int32_t value);
#endif /* EFFECTS_DELAY_BASED_VARIABLE_DELAY_H_ */
