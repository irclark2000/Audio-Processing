/*
 * effect_component.h
 *
 *  Created on: Oct 21, 2024
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


#ifndef EFFECTS_TESTER_AUDIO_PLAYER_EFFECT_COMPONENT_H_
#define EFFECTS_TESTER_AUDIO_PLAYER_EFFECT_COMPONENT_H_

#include <stdint.h>
#include "apply_effect.h"

#if AUDIO_EFFECTS_TESTER
#include <string.h>
#include <stdlib.h>
#endif
struct EFFECT_COMPONENT;
#define MAX_CHILD_EFFECT_COMPONENTS 10
typedef enum {
	Lfo, Mixer, VariableDelay, VariableBandpass,
	FirstOrderAllPass, CircularBuffer, FeedBackCombFilter,
	Echo, WahWah, Chorus, Flanger, Vibrato, ChorusElement,
	Freeverb, Schroeder, Compressor, Expander, Limiter, Noisegate,
	Phaser, Overdrive, PitchShift, Tremolo,

} EFFECT_TYPE;

typedef struct EFFECT_COMPONENT {
	EFFECT_TYPE type;
	void * effect;
	void (*initialize) (void *type, EFFECT_PARAMS *parameters, float sampleRate);
	float (*apply) (void *, float);
	char *strParameters[10];
	EFFECT_PARAMS *parameters;
	uint8_t parameterCount;
	struct EFFECT_COMPONENT *childComponents[MAX_CHILD_EFFECT_COMPONENTS];
	uint8_t childrenCount;
	uint8_t effect_bypass;
} EFFECT_COMPONENT;
char * strSave (char * string);
void do_nothing0_Component(void * effect);
void do_nothing1_Component(void * effect, EFFECT_PARAMS *parameters,float sampleRate);
EFFECT_COMPONENT * createComponent(char *effectName, char *strParameters);
void freeComponent (EFFECT_COMPONENT *component);
#endif /* EFFECTS_TESTER_AUDIO_PLAYER_EFFECT_COMPONENT_H_ */
