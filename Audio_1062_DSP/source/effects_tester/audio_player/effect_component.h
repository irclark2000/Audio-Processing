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
#include <string.h>
#include <stdlib.h>

struct EFFECT_COMPONENT;
#define MAX_CHILD_EFFECT_COMPONENTS 15
typedef enum {
	Lfo, Mixer, VariableDelay, VariableBandpass,
	FirstOrderAllPass, CircularBuffer, FeedBackCombFilter, StateVariableFilter,
	EnvelopeFollower, Volume, EqualizingFilter,
	AutoWah, Echo, WahWah, Chorus, Flanger, Vibrato, ChorusElement,
	Freeverb, Schroeder, Asymmetric_Overdrive, Moorer, Compressor, Expander, Limiter, Noisegate,
	Phaser, Overdrive, PitchShift, Tremolo, Equalizer, None

} EFFECT_TYPE;

typedef float (*APPLY) (void *type, float);
typedef void (*RECALCULATE) (void *effect);
typedef void (*UNINITIALIZE) (void *effect);

typedef struct EFFECT_COMPONENT {
	EFFECT_TYPE type;
	char *effectName;
	void * effect;
	uint8_t effect_count;
	void (*uninitialize) (void *type);
	//void (*initialize) (void *type, EFFECT_PARAMS *parameters, float sampleRate);
	float (*apply) (void *, float);
	char *strParameters[10];
	char *strTypes[10];
	EFFECT_PARAMS *parameters;
	// identify primary effect 
	// to avoid calling free on children effects
	uint8_t main_effect;  
	uint8_t parameterCount;
	struct EFFECT_COMPONENT *childComponents[MAX_CHILD_EFFECT_COMPONENTS];
	uint8_t childrenCount;
	uint8_t effect_bypass;   // disable effect during playback
	float volume;
} EFFECT_COMPONENT;
char * strSave (char * string);
void do_nothing0_Component(void * effect);
void do_nothing1_Component(void * effect, EFFECT_PARAMS *parameters,float sampleRate);
EFFECT_COMPONENT * createComponent(char *effectName, char *strParameters, void *effect);
void addChildComponent(EFFECT_COMPONENT * component, char * componentName);
void freeComponent (EFFECT_COMPONENT *component);
#endif /* EFFECTS_TESTER_AUDIO_PLAYER_EFFECT_COMPONENT_H_ */
