/*
 * effects_gui_interface.c
 *
 *  Created on: Oct 24, 2024
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
#include "effects_gui_interface.h"
#include "effects/delay_based/echo.h"
#include <string.h>
#if AUDIO_EFFECTS_TESTER
#include <stdlib.h>
#endif

EFFECT_ITEM delay_based_effect[] = {
		{"Echo", Echo}, {"Flanger", Flanger}, {"Vibrato", Vibrato},
		{"Chorus", Chorus}
};
EFFECT_ITEM reverb_effect[] = {
		{"Freeverb", Freeverb}, {"Schroeder Reverb", Schroeder}
};
EFFECT_ITEM dynamic_range_control_effect[] = {
		{"Compressor", Compressor}, {"Expander", Expander},
		{"Limiter", Limiter}, {"Noise Gate", Noisegate}
};
EFFECT_ITEM misc_effect[] = {
		{"Phaser", Phaser}, {"Overdrive", Overdrive},
		{"Pitch Shifter", PitchShift}, {"Tremolo", Tremolo},{"Wah Wah", WahWah},
};

EFFECT_ITEM effects_list[] = {
	{"Echo", Echo}, {"Flanger", Flanger}, {"Vibrato", Vibrato},
	{"Chorus", Chorus}
};

float applyEffect_COMPONENT(void *type, EFFECT_COMPONENT *effect, float input) {
	return 0;
}

void setParameter_COMPONENT(void *type, EFFECT_COMPONENT *effect, char *pName, float value) {
	return;
}

float getParameter_COMPONENT(void *type, EFFECT_COMPONENT *effect, char *pName) {
	return 0.0f;
}
/*
 * typedef struct {
	VARDELAY vDelay;
	float feedBack_level;
	float feedback_gain;
	float out;
	MIXER mixer;
} ECHO;
 *
 */
/*
 * typedef struct {
	EFFECT_TYPE type;
	void * effect;
	void (*initialize) (void *, EFFECT_PARAMS *parameters, float sampleRate);
	float (*apply) (void *, float);
	void (*uninitialize) (void *);
	char component_parameters[5][80];
	EFFECT_PARAMS *parameters;
	uint8_t parameterCount;
	EFFECT_COMPONENT *childComponents[MAX_CHILD_EFFECT_COMPONENTS];
	uint8_t childrenCount;
	uint8_t effect_bypass;
} EFFECT_COMPONENT;
 *
 */
