/*
 * variable_delay.h
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

#ifndef EFFECTS_COMPONENTS_VARIABLE_DELAY_H_
#define EFFECTS_COMPONENTS_VARIABLE_DELAY_H_
#if AUDIO_EFFECTS_TESTER
#include "effects_tester/audio_player/effect_component.h"
#endif
#include <components/circular_buffer.h>
#include <stdint.h>
typedef struct {
	float max_delay;  // seconds
	float delayInSamples;
	float sampleTime;
	float sampleRate;
	float out;
	uint32_t size;
	CIRCBUFFER cBuf;
	CIRCBUFFER *cBufPtr;
}VARDELAY;

void initialize_variable_delay (VARDELAY * vDelay, float *buf, float buf_size, float sampleRate);
float getDelayedSample_VARDELAY(VARDELAY *vDelay, float input, float fb_level);
void setDelay_VARDELAY(VARDELAY *vDelay, float delaySec);
void setReadPointer(VARDELAY *vDelay, int32_t value);
float getFloatAtIndex_VARDELAY (VARDELAY *vDelay, int32_t index);
float getFloatAtReadPtrWithIndex_VARDELAY(VARDELAY *vDelay, uint32_t index);
void uninitialize_VARDELAY(VARDELAY *vDelay);
#if AUDIO_EFFECTS_TESTER
void uninitialize_VARDELAY(VARDELAY *vDelay);
void initialize_componentVARDELAY(void *vvDelay, EFFECT_PARAMS * parameters, float sampleRate);
void set_componentVARDELAY(void *vvDelay, EFFECT_PARAMS * parameters, float newValue);
float get_componentVARDELAY(void *vvDelay, EFFECT_PARAMS * parameters);
#endif

#endif /* EFFECTS_COMPONENTS_VARIABLE_DELAY_H_ */
