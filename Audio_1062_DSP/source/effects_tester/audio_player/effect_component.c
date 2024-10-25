/*
 * effect_component.c
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
/*
 * 	EFFECT_TYPE type;
	void * effect;
	void (*initialize) (void *, EFFECT_PARAMS *parameters, float sampleRate);
	float (*apply) (void *, float);
	void (*uninitialize) (void *);
	char component_parameters[5;
	EFFECT_PARAMS *parameters;
	uint8_t parameterCount;
	struct EFFECT_COMPONENT *childComponents[MAX_CHILD_EFFECT_COMPONENTS];
	uint8_t childrenCount;
	uint8_t effect_bypass;
 */

#include "effect_component.h"
#include "effects/delay_based/echo.h"
#include "effects/components/mixer.h"

EFFECT_COMPONENT * createComponent(char *effectName, char *strParameters) {
#if AUDIO_EFFECTS_TESTER
	EFFECT_COMPONENT *component = (EFFECT_COMPONENT *)malloc(sizeof(EFFECT_COMPONENT));
	if (strcmp(effectName, "Echo") == 0) {
		component->type = Echo;
		component->parameterCount = 2;
		component->parameters = (EFFECT_PARAMS *)malloc (sizeof(EFFECT_PARAMS));
		int index = 0;
		component->strParameters[0] = "Feedback Gain:S3";
		component->parameters->floatParameter[index++] = 0.0f;
		component->parameters->floatParameter[index++] = 0.4f;
		component->parameters->floatParameter[index++] = 1.0f;
		component->strParameters[1] = "Feedback Level:S3";
		component->parameters->floatParameter[index++] = 0.0f;
		component->parameters->floatParameter[index++] = 0.35f;
		component->parameters->floatParameter[index++] = 0.5f;
		component->childrenCount = 2;
		index = 0;
		component->childComponents[index++] = createComponent("Mixer", 0);
		component->childComponents[index++] = createComponent("Variable Delay", "Delay MSec:S3-0,250,500");
		component->apply = update_Echo;
		component->effect_bypass = 0;
	}

	else if (strcmp(effectName, "Mixer") == 0) {
		component->type = Mixer;
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS *) malloc (sizeof(EFFECT_PARAMS));
		int index = 0;
		component->strParameters[0] = "Wet/Dry:S3";
		component->parameters->floatParameter[index++] = 0.0f;
		component->parameters->floatParameter[index++] = 0.4f;
		component->parameters->floatParameter[index++] = 1.0f;
		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;
	}
	else if (strcmp(effectName, "Variable Delay") == 0) {
		char temp[80];
		char *ptr = strtok(temp, "-");
		component->strParameters[0] = strdup(temp);

		component->type = VariableDelay;
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS *) malloc (sizeof(EFFECT_PARAMS));
		int index = 0;
		ptr = strtok(NULL, ":");
		component->parameters->floatParameter[index++] = atof(ptr);
		ptr = strtok(NULL, ":");
		component->parameters->floatParameter[index++] = atof(ptr);
		ptr = strtok(NULL, ":");
		component->parameters->floatParameter[index++] = atof(ptr);
		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;
	}
	else {
		free(component);
		component = 0;
	}
	return component;
#else
	return 0;
#endif
}
void do_nothing_Component(void * effect) {
	return;
}
