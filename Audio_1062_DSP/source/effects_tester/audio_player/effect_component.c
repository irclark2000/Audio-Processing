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

#include "effect_component.h"
#include "effects/delay_based/echo.h"
#include "effects/delay_based/flanger.h"
#include "effects/delay_based/chorus.h"
#include "effects/delay_based/vibrato.h"
#include "effects/variable_filter_effects/wah_wah.h"
#include "effects/variable_filter_effects/auto_wah.h"
#include "effects/components/mixer.h"
#include "effects/components/low_frequency_oscillator.h"
#include "effects/components/variable_bandpass_filter.h"
#include "effects/components/variable_delay.h"
#include <ctype.h>
#if !AUDIO_EFFECTS_TESTER

#endif

#if AUDIO_EFFECTS_TESTER
static char* parseParameters(char *ptr, EFFECT_PARAMS *parameters,
		uint8_t count) {
	ptr = strtok(ptr, ",");
	for (int i = 0; i < count; i++) {
		parameters->floatParameter[i] = atof(ptr);
		ptr = strtok(NULL, ",");
	}
	return ptr;
}

static void setName_Type_Parse_Variables (EFFECT_COMPONENT *component, uint8_t index, char *name_type) {
   char *ptr = name_type;
   char temp[80];
   int len = 0;
   while (*ptr != 0 && *ptr != ':') {
	   temp[len] = *ptr;
	   len++;
	   ptr++;
   }
   temp[len] = 0;
   component->strParameters[index] = strSave(temp);
   ptr = strtok(ptr, "*");
   if (*ptr != 0) {
	   component->strTypes[index] = strSave(ptr + 1);
   } else {
	   component->strTypes[index] = strSave("");
   }
   uint8_t count = 0;
   char *ptr2 = strtok(NULL, "*");
   switch (*(ptr+1)) {
	   case 'S':
		   count = atoi(ptr + 2);
		   for (int i=0; i < count; i++) {
			   if (i == 0) ptr2 = strtok(ptr2, ",");
			   else ptr2 = strtok(NULL, ",");
			   component->parameters[index].floatParameter[i] = atof(ptr2);
		   }
		   break;
	   case 'I':
		   component->parameters[index].intParameter[0] = atoi(ptr2);
		   break;
	   case 'X':
		   component->parameters[index].floatParameter[0] = atof(ptr2);
		   break;

   }
}

static void setName_Type (EFFECT_COMPONENT *component, uint8_t index, char *name_type) {
	char *ptr = name_type;
	char temp[80];
	int len = 0;
	while (*ptr != 0 && *ptr != ':') {
		temp[len] = *ptr;
		len++;
		ptr++;
	}
	temp[len] = 0;
	component->strParameters[index] = strSave(temp);
	if (*ptr != 0) {
		component->strTypes[index] = strSave(ptr + 1);
	} else {
		component->strTypes[index] = strSave("");
	}
}
void freeComponent(EFFECT_COMPONENT * component) {
	free(component->effectName);
	for (int i=0; i < component->parameterCount; i++) {
		free(component->strParameters[i]);
		free(component->strTypes[i]);
		if (i == 0) {
			free (component->parameters);
		}
	}
	for (int i=0; i < component->childrenCount; i++) {
		freeComponent(component->childComponents[i]);
	}
}
#endif
EFFECT_COMPONENT * createComponent(char *effectName, char *strParameters, void *effect) {
#if AUDIO_EFFECTS_TESTER
	EFFECT_COMPONENT *component = (EFFECT_COMPONENT*) malloc(
			sizeof(EFFECT_COMPONENT));
	component->effectName = strSave(effectName);
	component->apply = 0;
	/*
	 * 	float awOut;
	 float sampleRate;
	 ENVELOPE_FOLLOWER ef;
	 MIXER mixer;
	 VARBANDPASS vbf;
	 float inputGain;
	 float fxGain;
	 float minCoFreq;
	 float maxCoFreq;
	 *
	 */
	if (strcmp(effectName, "Auto Wah") == 0) {
		AUTOWAH *aw = (AUTOWAH *) malloc(sizeof(AUTOWAH));
		component->effect = aw;
		component->type = AutoWah;
		component->parameters = (EFFECT_PARAMS*) malloc(4 * sizeof(EFFECT_PARAMS));
		char temp[480];
		if (strParameters == 0) {
			char *elements= "InputGain:S3*0.01,1,10\tFxGain:S3*0.01,1,10\tMin Cutoff Freq:S3*10,267,500\tMax Cutoff Freq:S3*510,600,2000//Q:S3*0.7,4,10\tCutoff Freq:X*1000\tPass:I*1";
			strcpy(temp, elements);
		}
		else {
			strcpy(temp, strParameters);
		}
		char *ptrParameters = strtok(temp, "//");
		char *ptrVarBandPass = strtok(NULL, "//");
		// 4 parameters
		component->parameterCount = 4;
		char *params[4];
		char *ptr = strtok(ptrParameters, "\t");
		for (int i=0; i < component->parameterCount; i++) {
			params[i] = ptr;
			ptr = strtok(NULL, "\t");
		}
		for (int i=0; i < component->parameterCount; i++) {
			setName_Type_Parse_Variables (component, i, params[i]);
		}
		component->parameters[0].currentValue = &(aw->inputGain);
		component->parameters[1].currentValue = &(aw->fxGain);		
		component->parameters[2].currentValue = &(aw->minCoFreq);
		component->parameters[3].currentValue = &(aw->maxCoFreq);		

		// children
		component->childrenCount = 3;
		component->childComponents[0] = createComponent("Variable BandPass", ptrVarBandPass, &(aw->vbf));
		component->childComponents[1] = createComponent("Envelope Follower", 0, &(aw->ef));
		component->childComponents[2] = createComponent("Mixer", 0, &(aw->mixer));
		component->apply = apply_AUTOWAH;
		component->effect_bypass = 0;
	}
	else if (strcmp(effectName, "Chorus") == 0) {
		CHORUS *chorus = (CHORUS *) malloc(sizeof(CHORUS));
		component->effect = chorus;
		component->type = Chorus;
		component->parameterCount = 0;
		component->childrenCount = 0;
		char temp[480];
		// forced order: base delay, then Lfo, then Lfo Driven Delay
		if (strParameters == 0) {
			char * elements = "Count:I*2//BaseDelay 1:S3*0,1,10\nLFO 1 Freq:S3*0.1,1,5\tLFO 1 Depth (mSec):S3*0,1,10\nDelay 1 Max:X*30//BaseDelay 2:S3*0,1,10\nLFO 2 Freq:S3*0.1,1,5\tLFO 2 Depth (mSec):S3*0,1,10\nDelay 2 Max:X*30";
			strcpy(temp, elements);
		}
		else {
			strcpy(temp, strParameters);
		}
		char * ptrCount = strtok(temp, "//");
		while(*ptrCount != ':' && *ptrCount !=0) ptrCount++;
		if (*ptrCount == 0 || *(ptrCount + 1) != 'I') {
			freeComponent(component);
			return 0;
		}
		while(*ptrCount != '*' && *ptrCount !=0) ptrCount++;
		if (*ptrCount == 0 || !isdigit(*(ptrCount + 1))) {
			freeComponent(component);
			return 0;
		}
		int count = atoi(ptrCount + 1);
			chorus->chorus_count =  count;
			chorus->inv_count = 1.0f / count;
		int componentCount = chorus->chorus_count;
		char *child[12];  // max allowed childen;
		for (int i = 0; i < componentCount; ++i) {
			child[i] = strtok(NULL, "//");
		}
		for (int i = 0; i < componentCount; ++i) {
			EFFECT_COMPONENT * cmp =
				createComponent("Chorus Element", child[i], chorus->cElement + i);
			component->childComponents[i] = cmp;
		}

		component->childComponents[componentCount++] = createComponent("Mixer", 0, &(chorus->mixer));
		component->childrenCount = componentCount;
		component->apply = update_CHORUS;
		component->effect_bypass = 0;
	}
	else if (strcmp(effectName, "Echo") == 0) {
		component->type = Echo;
		ECHO *echo = (ECHO *) malloc(sizeof(ECHO));
		component->effect = echo;
		component->parameterCount = 2;
		component->parameters = (EFFECT_PARAMS*) malloc(
				2 * sizeof(EFFECT_PARAMS));
		int index = 0;
		setName_Type(component, 0, "Feedback Gain:S3");
		component->parameters[0].floatParameter[index++] = 0.0f;
		component->parameters[0].floatParameter[index++] = 0.4f;
		component->parameters[0].floatParameter[index++] = 1.0f;
		setName_Type(component, 1, "Feedback Level:S3");
		component->parameters[0].currentValue = &(echo->feedback_gain);

		index = 0;
		component->parameters[1].floatParameter[index++] = 0.0f;
		component->parameters[1].floatParameter[index++] = 0.35f;
		component->parameters[1].floatParameter[index++] = 0.5f;
		component->parameters[1].currentValue = &(echo->feedback_level);
		component->childrenCount = 2;
		index = 0;
		component->childComponents[index++] = createComponent("Mixer", 0, &(echo->mixer));
		component->childComponents[index++] = createComponent("Variable Delay",
				"Delay mSec:S3*0,250,500", &(echo->vDelay));
		component->apply = update_Echo;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Flanger") == 0) {
		component->type = Flanger;
		FLANGER *flng = (FLANGER *) malloc(sizeof(FLANGER));
		component->effect = flng;
		component->parameterCount = 2;
		component->parameters = (EFFECT_PARAMS*) malloc(2 * sizeof(EFFECT_PARAMS));
		int index = 0;
		setName_Type(component, 0, "Base Delay MSec:S3");
		component->parameters[0].floatParameter[index++] = 0.1f;
		component->parameters[0].floatParameter[index++] = 3.0f;
		component->parameters[0].floatParameter[index++] = 10.0f;
		setName_Type(component, 1, "Feedback Level:S3");
		component->parameters[0].currentValue = &(flng->baseDelay_MSec);
		index = 0;
		component->parameters[1].floatParameter[index++] = 0.0f;
		component->parameters[1].floatParameter[index++] = 0.35f;
		component->parameters[1].floatParameter[index++] = 0.98f;
		component->parameters[1].currentValue = &(flng->feedback_level);
		component->childrenCount = 3;
		index = 0;
		component->childComponents[index++] = createComponent("Mixer", 0, &(flng->mixer));
		component->childComponents[index++] = createComponent("Variable Delay",
				"Delay mSec:X*30", &(flng->vDelay));
		component->childComponents[index++] = createComponent("Lfo",
				"LFO Freq:S3*0.1,1,5\tLFO Depth (mSec):S3*0,1,10", 0);
		component->apply = update_FLANGER;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Vibrato") == 0) {
		component->type = Vibrato;
		VIBRATO *vib = (VIBRATO *) malloc(sizeof(VIBRATO));
		component->effect = vib;
		char temp[160];
		if (strParameters == 0) {
			char * elements = "Base Delay (MS):S3*0.1,5,14//Max Delay:X*15//LFO Frequency:S3*0.1,5,15\tLFO Depth (mSec):S3*0,2.5,5";
			strcpy(temp, elements);
		}
		else {
			strcpy(temp, strParameters);
		}
		char *ptrBaseDelay = strtok(temp, "//");
		char *ptrVarDelay = strtok(NULL, "//");
		char *ptrLFO = strtok(NULL, "//");

		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS*) malloc(sizeof(EFFECT_PARAMS));
		char *ptr = strtok (ptrBaseDelay, "*");
		setName_Type(component, 0, ptr);
		while(*ptr != ':' && *ptr != 0) ptr++;
		if (*ptr == ':' && *(ptr + 1) == 'S') {
			uint8_t count = atoi(ptr + 2);
			ptr = strtok(NULL, "*");
			ptr = parseParameters(ptr, component->parameters, count);
		} else {
			freeComponent(component);
		}
		component->parameters[0].currentValue = &(vib->baseDelayMSec);
		component->childrenCount = 3;
		component->childComponents[0] = createComponent("Lfo", ptrLFO, &(vib->lfo));
		component->childComponents[1] = createComponent("Variable Delay", ptrVarDelay, &(vib->vDelay));
		component->childComponents[2] = createComponent("Mixer", 0, &(vib->mixer));
		component->apply = apply_VIBRATO;
		component->effect_bypass = 0;

	} else if (strcmp(effectName, "Wah Wah") == 0) {
		component->type = WahWah;
		char temp[160];
		if (strParameters == 0) {
			char * elements = "Q:X*6\tCenter Frequency:S3*500,2000,5000"
				"//LFO Freq:S3*0.1,2000,3000\tLFO Depth (Hz):S3*0,250,1000";
			strcpy(temp, elements);
		}
		else {
			strcpy(temp, strParameters);
		}
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS*) malloc(sizeof(EFFECT_PARAMS));
		char *ptr = strtok (temp, "*");
		setName_Type(component, 0, ptr);
		while(*ptr != ':' && *ptr != 0) ptr++;
		if (*ptr == ':' && *(ptr + 1) == 'S') {
			uint8_t count = atoi(ptr + 2);
			ptr = strtok(NULL, "*");
			ptr = parseParameters(ptr, component->parameters, count);
		} else {
			freeComponent(component);
		}
	} else if (strcmp(effectName, "Chorus Element") == 0) {
		CHORUSELEMENT *cElement;
		if (effect) {
			cElement = (CHORUSELEMENT *) effect;
		}
		else {
			cElement = (CHORUSELEMENT *) malloc(sizeof(CHORUSELEMENT));
			component->effect = cElement;
		}
		component->type = ChorusElement;
		char temp[160];
		strcpy(temp, strParameters);
		// forced order: base delay, then Lfo, then Lfo Driven Delay
		char *ptrBaseDelay = strtok(temp, "\n");  // baseDelay
		char *ptrLfo = strtok(NULL, "\n");  // LFO
		char *ptrVarDelay = strtok(NULL, "\n");  // Driven VARDELAY
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS*) malloc(sizeof(EFFECT_PARAMS));
		setName_Type_Parse_Variables (component, 0, ptrBaseDelay);
		component->parameters[0].currentValue = &(cElement->baseDelayMSec);
		component->childrenCount = 2;
		component->childComponents[0] = createComponent("Lfo", ptrLfo, &(cElement->lfo));
		component->childComponents[1] = createComponent("Variable Delay", ptrVarDelay, &(cElement->vDelay));
	}
	else if (strcmp(effectName, "Envelope Follower") == 0) {
		component->type = EnvelopeFollower;
		// has no parametersp and no children
		component->parameterCount = 0;
		component->childrenCount = 0;
		component->apply = 0;
	}
	else if (strcmp(effectName, "Lfo") == 0) {
		component->type = Lfo;
		LOWFREQOSC *lfo;
		if (effect) {
			lfo = (LOWFREQOSC *) effect;
		}
		else {
			lfo = (LOWFREQOSC *) malloc(sizeof(LOWFREQOSC));
			component->effect = lfo;
		}
		// must have 2 parameters frequency and Depth
		char temp[80];
		int sine = 0;
		int phase = 0;
		strcpy(temp, strParameters);
		char *ptrFreq = strtok(temp, "\t");
		char *ptrDepth = strtok(NULL, "\t");
		char *ptrSine = strtok(NULL, "\t");
		component->parameters = (EFFECT_PARAMS*) malloc(2 * sizeof(EFFECT_PARAMS));
		setName_Type_Parse_Variables (component, 0, ptrFreq);
		component->parameters[0].currentValue = &(lfo->oscFreq);
		setName_Type_Parse_Variables (component, 1, ptrDepth);
		component->parameters[1].currentValue = &(lfo->amplitude);
		if (ptrSine) {
			while(*ptrSine != ':' && *ptrSine != 0) ptrSine++;
			if (*ptrSine == ':' && *(ptrSine + 1) == 'I') {
				sine  = atoi(ptrSine + 2);
			}	
		}
		lfo->triangle_sine_select = sine;
		lfo->phaseAngleDeg = phase;

		component->parameterCount = 2;
		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Mixer") == 0) {
		component->type = Mixer;
		MIXER *mixer = (MIXER *) malloc(sizeof(MIXER));
		component->effect = mixer;
		component->type = Mixer;
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS*) malloc(sizeof(EFFECT_PARAMS));
		int index = 0;
		setName_Type(component, 0, "Wet/Dry:S3");
		component->parameters->floatParameter[index++] = 0.0f;
		component->parameters->floatParameter[index++] = 0.6f;
		component->parameters->floatParameter[index++] = 1.0f;
		component->parameters[0].currentValue = &(mixer->wet_dry);
		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "State Variable Filter") == 0) {
		component->type = StateVariableFilter;
		char temp[160];
		if (strParameters == 0) {
			char * elements = "Q:X*6\tCenter Frequency:S3*500,2000,5000"
				"//LFO Freq:S3*0.1,2000,3000\tLFO Depth (Hz):S3*0,250,1000";
			strcpy(temp, elements);
		}
		else {
			strcpy(temp, strParameters);
		}
		//		char *ptr = strtok(temp, "*");
		component->parameterCount = 2;
		component->parameters = (EFFECT_PARAMS*) malloc(
				2 * sizeof(EFFECT_PARAMS));
		char *ptr1 = strtok(temp, "//");
		char *ptr2 = strtok(NULL, "//");
		char parameter1[80];
		char parameter2[80];
		strcpy(parameter1, ptr1);
		strcpy(parameter2, ptr2);


	} else if (strcmp(effectName, "Variable BandPass") == 0) {
		component->type = VariableDelay;
		VARBANDPASS *vbp = (VARBANDPASS *) malloc(sizeof(VARBANDPASS));
		component->effect = vbp;
		char temp[80];
		strcpy(temp, strParameters);
		char *ptrQ = strtok(temp, "\t");
		char *ptrCutoff = strtok(NULL, "\t");
		char *ptrPassBlock = strtok(NULL, "\t");
		component->parameterCount = 3;
		component->parameters = (EFFECT_PARAMS*) malloc(
				3 * sizeof(EFFECT_PARAMS));
		// three parameters
		setName_Type_Parse_Variables (component, 0, ptrQ);
		component->parameters[0].currentValue = &(vbp->gui_Q);
		setName_Type_Parse_Variables (component, 1, ptrCutoff);
		component->parameters[1].currentValue = &(vbp->gui_Freq);
		setName_Type_Parse_Variables (component, 2, ptrPassBlock);
		vbp->pass_stop = component->parameters[2].intParameter[0];
		// no children
		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;

	} else if (strcmp(effectName, "Variable Delay") == 0) {
		component->type = VariableDelay;
		VARDELAY *vDelay;
		if (effect) {
			vDelay = (VARDELAY *) effect;
		}
		else {
			vDelay = (VARDELAY *) malloc(sizeof(VARDELAY));
			component->effect = vDelay;
		}
		char temp[80];
		strcpy(temp, strParameters);
		char *ptr = strtok(temp, "*");
		setName_Type(component, 0, temp);
		component->parameterCount = 1;
		component->parameters = (EFFECT_PARAMS*) malloc(
				sizeof(EFFECT_PARAMS));
		while(*ptr != ':' && *ptr != 0) ptr++;
		float maxDelay;
		if (*ptr == ':' && *(ptr + 1) == 'X') {
			ptr = strtok(NULL, "*");
			maxDelay = component->parameters->floatParameter[0] = atof(ptr);
		}
		else if (*ptr == ':' && *(ptr + 1) == 'S') {
			uint8_t count = atoi (ptr + 2);
			ptr = strtok(NULL, "*");
			ptr = parseParameters(ptr, component->parameters, count);
			maxDelay = component->parameters->floatParameter[2];
			component->parameters[0].currentValue = &(vDelay->max_delay);
		} else {
			freeComponent(component);
			return 0;
		}
		vDelay->max_delay = maxDelay/1000.0f;
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
