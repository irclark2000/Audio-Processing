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
#include "effects/delay_based/flanger.h"
#include "effects/delay_based/chorus.h"
#include "effects/delay_based/vibrato.h"
#include "effects/variable_filter_effects/wah_wah.h"
#include "effects/components/mixer.h"
#include "effects/components/low_frequency_oscillator.h"
#include "effects/components/state_variable_filter.h"

#if AUDIO_EFFECTS_TESTER
static char* parseParameters(char *ptr, EFFECT_PARAMS *parameters,
		uint8_t count) {
	int index = 0;
	ptr = strtok(ptr, ",");
	for (int i = 0; i < count; i++) {
		parameters->floatParameter[i] = atof(ptr);
		ptr = strtok(NULL, ",");
	}
	return ptr;
}
void freeComponent(EFFECT_COMPONENT * component) {
	free(component->effectName);
	for (int i=0; i < component->parameterCount; i++) {
		free(component->strParameters[i]);
		if (i == 0) {
			free (component->parameters);
		}
	}
	for (int i=0; i < component->childrenCount; i++) {
		freeComponent(component->childComponents[i]);
	}
}
#endif
EFFECT_COMPONENT* createComponent(char *effectName, char *strParameters) {
#if AUDIO_EFFECTS_TESTER
	EFFECT_COMPONENT *component = (EFFECT_COMPONENT*) malloc(
			sizeof(EFFECT_COMPONENT));
	component->effectName = strSave(effectName);
	if (strcmp(effectName, "Chorus") == 0) {
		component->type = Chorus;
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS*) malloc(sizeof(EFFECT_PARAMS));
		char temp[480];
		// forced order: base delay, then Lfo, then Lfo Driven Delay
		if (strParameters == 0) {
			char * elements = "BaseDelay 1:S3*0,1,10//LFO 1 Freq:S3*0.1,1,5\tLFO 1 Depth(MSec):S3*0,1,10//Delay 1 Max:X*30\nBaseDelay 2:S3*0,1,10//LFO 2 Freq:S3*0.1,1,5\tLFO 2 Depth(MSec):S3*0,1,10//Delay 2 Max:X*30";
			strcpy(temp, elements);
		}
		else {
			strcpy(temp, strParameters);
		}
		int componentCount = 0;
		char *ptrStart = temp;
		char *ptrEnd = temp;
		while(*ptrEnd != 0 && *ptrEnd != '\n') ptrEnd++;
		int more = 1;
		if (*ptrEnd == 0) {
			more = 0;
		}
		*ptrEnd = 0;
		while (*ptrStart) {
			char temp2[80];
			EFFECT_COMPONENT * cmp =
					createComponent("Chorus Element", ptrStart);
			if(cmp != NULL) {
				component->childComponents[componentCount] = cmp;
				componentCount++;
				if (more) {
					ptrStart = ptrEnd + 1;
					ptrEnd = ptrStart;
					while(*ptrEnd != 0 && *ptrEnd != '\n') ptrEnd++;
					if (*ptrEnd == 0) {
						more = 0;
					}
					*ptrEnd = 0;
				} else {
					break;
				}
			}
			else {
				freeComponent(component);
				return 0;
			}
		}
		component->childComponents[componentCount++] = createComponent("Mixer", 0);
		component->childrenCount = componentCount;
		component->apply = update_CHORUS;
		component->effect_bypass = 0;
		component->strParameters[0] = strSave("Chorus Element Count:I1");
		component->parameters[0].intParameter[0] = 2;

	}
	else if (strcmp(effectName, "Echo") == 0) {
		component->type = Echo;
		component->parameterCount = 2;
		component->parameters = (EFFECT_PARAMS*) malloc(
				2 * sizeof(EFFECT_PARAMS));
		int index = 0;
		component->strParameters[0] = strSave("Feedback Gain:S3");
		component->parameters[0].floatParameter[index++] = 0.0f;
		component->parameters[0].floatParameter[index++] = 0.4f;
		component->parameters[0].floatParameter[index++] = 1.0f;
		component->strParameters[1] = strSave("Feedback Level:S3");
		index = 0;
		component->parameters[1].floatParameter[index++] = 0.0f;
		component->parameters[1].floatParameter[index++] = 0.35f;
		component->parameters[1].floatParameter[index++] = 0.5f;
		component->childrenCount = 2;
		index = 0;
		component->childComponents[index++] = createComponent("Mixer", 0);
		component->childComponents[index++] = createComponent("Variable Delay",
				"Delay MSec:S3*0,250,500");
		component->apply = update_Echo;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Flanger") == 0) {
		component->type = Flanger;
		component->parameterCount = 2;
		component->parameters = (EFFECT_PARAMS*) malloc(2 * sizeof(EFFECT_PARAMS));
		int index = 0;
		component->strParameters[0] = strSave("Base Delay MSec:S3");
		component->parameters[0].floatParameter[index++] = 0.1f;
		component->parameters[0].floatParameter[index++] = 3.0f;
		component->parameters[0].floatParameter[index++] = 10.0f;
		component->strParameters[1] = strSave("Feedback Level:S3");
		index = 0;
		component->parameters[1].floatParameter[index++] = 0.0f;
		component->parameters[1].floatParameter[index++] = 0.35f;
		component->parameters[1].floatParameter[index++] = 0.98f;
		component->childrenCount = 3;
		index = 0;
		component->childComponents[index++] = createComponent("Mixer", 0);
		component->childComponents[index++] = createComponent("Variable Delay",
				"Delay MSec:X*30");
		component->childComponents[index++] = createComponent("Lfo",
				"LFO Freq:S3*0.1,1,5\tLFO Depth(MSec):S3*0,1,10");
		component->apply = update_FLANGER;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Vibrato") == 0) {
		component->type = Vibrato;
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS*) malloc(sizeof(EFFECT_PARAMS));
		/*
		 * 	LOWFREQOSC lfo;
	PARAMETER_LIMITS lfo_freq_limits;
	VARDELAY vDelay;
	float baseDelayMSec;
	PARAMETER_LIMITS delay_limitsMsec;
	MIXER mixer;
		 *
		 */
		component->childrenCount = 3;
		component->apply = apply_VIBRATO;
		component->effect_bypass = 0;

	} else if (strcmp(effectName, "Wah Wah") == 0) {
		component->type = WahWah;
		char temp[160];
		if (strParameters == 0) {
			char * elements = "Q:X*6\tCenter Frequency:S3*500,2000,5000"
					"//LFO Freq:S3*0.1,2000,3000\tLFO Depth(Hz):S3*0,250,1000";
			strcpy(temp, elements);
		}
		else {
			strcpy(temp, strParameters);
		}
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS*) malloc(sizeof(EFFECT_PARAMS));
		char *ptr = strtok (temp, "*");
		component->strParameters[0] = strSave(ptr);
		while(*ptr != ':' && *ptr != 0) ptr++;
		if (*ptr == ':' && *(ptr + 1) == 'S') {
			uint8_t count = atoi(ptr + 2);
			ptr = strtok(NULL, "*");
			ptr = parseParameters(ptr, component->parameters, count);
		} else {
			freeComponent(component);
		}
	} else if (strcmp(effectName, "Chorus Element") == 0) {
		component->type = ChorusElement;
		char temp[160];
		strcpy(temp, strParameters);
		// forced order: base delay, then Lfo, then Lfo Driven Delay
		char *ptr1 = strtok(temp, "//");  // baseDelay
		char *ptr2 = strtok(NULL, "//");  // LFO
		char *ptr3 = strtok(NULL, "//");  // Driven VARDELAY
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS*) malloc(sizeof(EFFECT_PARAMS));
		char *ptr = strtok (ptr1, "*");
		component->strParameters[0] = strSave(ptr);
		while(*ptr != ':' && *ptr != 0) ptr++;
		if (*ptr == ':' && *(ptr + 1) == 'S') {
			uint8_t count = atoi(ptr + 2);
			ptr = strtok(NULL, "*");
			ptr = parseParameters(ptr, component->parameters, count);
		} else {
			freeComponent(component);
		}
		component->childrenCount = 2;
		component->childComponents[0] = createComponent("Lfo", ptr2);
		component->childComponents[1] = createComponent("Variable Delay", ptr3);
	}
	else if (strcmp(effectName, "Lfo") == 0) {
		component->type = Lfo;
		// must have 2 parameters frequency and Depth
		char temp[80];
		strcpy(temp, strParameters);
		char *ptr1 = strtok(temp, "\t");
		char *ptr2 = strtok(NULL, "\t");
		char parameter1[80];
		char parameter2[80];
		strcpy(parameter1, ptr1);
		strcpy(parameter2, ptr2);
		component->parameterCount = 2;
		component->parameters = (EFFECT_PARAMS*) malloc(2 * sizeof(EFFECT_PARAMS));
		char *ptr = strtok(parameter1, "*");
		component->strParameters[0] = strSave(ptr);
		while(*ptr != ':' && *ptr != 0) ptr++;
		if (*ptr == ':' && *(ptr + 1) == 'S') {
			uint8_t count = atoi(ptr + 2);
			ptr = strtok(NULL, "*");
			ptr = parseParameters(ptr, component->parameters, count);
		} else {
			freeComponent(component);
		}

		ptr = strtok(parameter2, "*");
		component->strParameters[1] = strSave(ptr);
		while(*ptr != ':' && *ptr != 0) ptr++;
		if (*ptr == ':' && *(ptr + 1) == 'S') {
			uint8_t count = atoi(ptr + 2);
			ptr = strtok(NULL, "*");
			ptr = parseParameters(ptr, component->parameters + 1, count);
		} else {
			freeComponent(component);
		}

		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Mixer") == 0) {
		component->type = Mixer;
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS*) malloc(sizeof(EFFECT_PARAMS));
		int index = 0;
		component->strParameters[0] = strSave("Wet/Dry:S3");
		component->parameters->floatParameter[index++] = 0.0f;
		component->parameters->floatParameter[index++] = 0.4f;
		component->parameters->floatParameter[index++] = 1.0f;
		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "State Variable Filter") == 0) {
		component->type = StateVariableFilter;
		char temp[160];
		if (strParameters == 0) {
			char * elements = "Q:X*6\tCenter Frequency:S3*500,2000,5000"
					"//LFO Freq:S3*0.1,2000,3000\tLFO Depth(Hz):S3*0,250,1000";
			strcpy(temp, elements);
		}
		else {
			strcpy(temp, strParameters);
		}
		char *ptr = strtok(temp, "*");
		component->parameterCount = 2;
		component->parameters = (EFFECT_PARAMS*) malloc(
				2 * sizeof(EFFECT_PARAMS));
		char *ptr1 = strtok(temp, "//");
		char *ptr2 = strtok(NULL, "//");
		char parameter1[80];
		char parameter2[80];
		strcpy(parameter1, ptr1);
		strcpy(parameter2, ptr2);


	} else if (strcmp(effectName, "Variable Delay") == 0) {
		component->type = VariableDelay;
		char temp[80];
		strcpy(temp, strParameters);
		char *ptr = strtok(temp, "*");
		component->strParameters[0] = strSave(temp);
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS*) malloc(
				sizeof(EFFECT_PARAMS));
		while(*ptr != ':' && *ptr != 0) ptr++;
		if (*ptr == ':' && *(ptr + 1) == 'X') {
			ptr = strtok(NULL, "*");
			component->parameters->floatParameter[0] = atof(ptr);
		}
		else if (*ptr == ':' && *(ptr + 1) == 'S') {
			uint8_t count = atoi (ptr + 1);
			ptr = strtok(NULL, "*");
			ptr = parseParameters(ptr, component->parameters, count);
		} else {
			freeComponent(component);
			return 0;
		}
		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;
	} else {
		freeComponent(component);
		component = 0;
	}
	return component;
#else
	return 0;
#endif
}
#if AUDIO_EFFECTS_TESTER
char* strSave(char *string) {
	char *cpy = (char*) malloc(strlen(string) + 1);
	strcpy(cpy, string);
	return cpy;
}
#endif
void do_nothing_Component(void *effect) {
	return;
}
