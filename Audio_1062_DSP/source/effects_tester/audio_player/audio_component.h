/*
 * audio_component.h
 *
 *  Created on: Dec 31, 2024
 *      Author: isaac
 */
/*
 Copyright 2024 Isaac R. Clark, Jr.

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
 fiiles (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy,
 modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef EFFECTS_TESTER_AUDIO_PLAYER_AUDIO_COMPONENT_H_
#define EFFECTS_TESTER_AUDIO_PLAYER_AUDIO_COMPONENT_H_

#include "effect_component.h"

#define MAX_SLIDER_COUNT 15
#define MAX_NUMBER_CHANNELS 6

// effects slider parameters
typedef struct SLIDER_FORMAT {
	char *slider_fmt;
	float slider_increment;
} SLIDER_FORMAT;

typedef enum CONTROL_TYPE {
	SLIDER, CHECKBOX, RADIOBUTTON
} CONTROL_TYPE;

typedef struct SLIDER_VALUES {
	EFFECT_PARAMS *myParameter;
	char *name;
	char *name0, *name1;
	CONTROL_TYPE control_type;
	int previousCheck;
	int slider_fmt_number;
	int *chkOutput;
	float slope;
	float intercept;
	float *slOutput;
	float previousOutput;
	float slider_value;  // 0.0->1.0;
} SLIDER_VALUES;

typedef struct AUDIO_COMPONENT {
	EFFECT_COMPONENT *channel[MAX_NUMBER_CHANNELS];
	uint8_t channel_count;
	uint8_t slider_count[MAX_NUMBER_CHANNELS];
	SLIDER_VALUES sliders[MAX_NUMBER_CHANNELS][MAX_SLIDER_COUNT];
}AUDIO_COMPONENT;

void initialize_AUDIO_COMPONENT (AUDIO_COMPONENT * ac);
void add_component(AUDIO_COMPONENT *ac, EFFECT_COMPONENT *ec);
void uninitialize_AUDIO_COMPONENT(AUDIO_COMPONENT *ac);
void clear_AUDIO_COMPONENT(AUDIO_COMPONENT *ac);
#endif /* EFFECTS_TESTER_AUDIO_PLAYER_AUDIO_COMPONENT_H_ */
