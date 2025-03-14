/*
 * effect_component.c
 *
 *  Created on: Oct 21, 2024
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

#include "effect_component.h"
#include "effects/tremolo.h"
#include "effects/overdrive.h"
#include "effects/delay_based/echo.h"
#include "effects/delay_based/flanger.h"
#include "effects/delay_based/chorus.h"
#include "effects/delay_based/vibrato.h"
#include "effects/dynamic_range_control/noise_gate.h"
#include "effects/dynamic_range_control/compressor.h"
#include "effects/dynamic_range_control/expander.h"
#include "effects/dynamic_range_control/limiter.h"
#include "effects/reverbs/freeverb.h"
#include "effects/reverbs/schroeder_verb.h"
#include "effects/reverbs/moorer_reverb.h"
#include "effects/variable_filter_effects/wah_wah.h"
#include "effects/variable_filter_effects/auto_wah.h"
#include "effects/variable_filter_effects/equalizer.h"
#include "effects/components/mixer.h"
#include "effects/components/volume_control.h"
#include "effects/components/low_frequency_oscillator.h"
#include "effects/components/variable_bandpass_filter.h"
#include "effects/components/variable_delay.h"
#include "effects/components/memory_debug.h"
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

static float setName_Type_Parse_Variables (EFFECT_COMPONENT *component, uint8_t index, char *name_type) {
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
   float value = 0;
   switch (*(ptr+1)) {
	   case 'S':
		   //count = atoi(ptr + 2);
		   count = 3;
		   for (int i=0; i < count; i++) {
			   if (i == 0) ptr2 = strtok(ptr2, ",");
			   else ptr2 = strtok(NULL, ",");
			   component->parameters[index].floatParameter[i] = atof(ptr2);
		   }
		   value = component->parameters[index].floatParameter[1];
		   break;
	   case 'R':
	   case 'C':
	   case 'I':
		   component->parameters[index].intParameter[0] = atoi(ptr2);
		   value = component->parameters[index].intParameter[0];
		   break;
	   case 'X':
		   component->parameters[index].floatParameter[0] = atof(ptr2);
		   value = component->parameters[index].floatParameter[0];
		   break;

   }
   return value;
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
	if (component == NULL) return;
	FREE(component->effectName);
	for (int i=0; i < component->parameterCount; i++) {
		FREE(component->strParameters[i]);
		FREE(component->strTypes[i]);
	}
	if (component->parameterCount) {
		FREE(component->parameters);
	}
	for (int i=0; i < component->childrenCount; i++) {
		freeComponent(component->childComponents[i]);
	}
	if (component->effect && component->main_effect) {
		if (component->uninitialize != 0) {
			component->uninitialize(component->effect);
		}
		free(component->effect);
	}
	FREE(component);
}

#endif
EFFECT_PARAMS* makeBlankParameters(uint8_t count, void *effect) {
	EFFECT_PARAMS *parameters = (EFFECT_PARAMS*) MALLOC(
			count * sizeof(EFFECT_PARAMS));
	for (uint8_t i = 0; i < count; ++i) {
		parameters[i].currentValue = 0;
		parameters[i].previousValue = 0;
		parameters[i].recalculate = 0;
		parameters[i].partnerParameter = 0;
		parameters[i].myEffect = effect;
	}
	return parameters;
}

void addChildComponent(EFFECT_COMPONENT *component, char *componentName) {
	EFFECT_COMPONENT *child = createComponent(componentName, 0, 0);
	if (child != NULL) {
		component->childComponents[component->childrenCount] = child;
		component->childrenCount++;
	}
}
EFFECT_COMPONENT* makeBlankComponent(char *effectName) {
	EFFECT_COMPONENT *component = (EFFECT_COMPONENT*) MALLOC(
			sizeof(EFFECT_COMPONENT));
	component->apply = 0;
	component->uninitialize = 0;
	component->parameters = 0;
	component->parameterCount = 0;
	component->childrenCount = 0;
	component->effect = 0;
	component->main_effect = 0;
	component->volume = 1.0f;
	if (effectName) {
		component->effectName = strSave(effectName);
	} else {
		component->effectName = 0;
	}
	return component;
}

EFFECT_COMPONENT* createComponent(char *effectName, char *strParameters,
		void *effect) {
#if AUDIO_EFFECTS_TESTER
	EFFECT_COMPONENT *component = makeBlankComponent(effectName);
	if (strcmp(effectName, "Auto Wah") == 0) {
		AUTOWAH *aw = (AUTOWAH*) MALLOC(sizeof(AUTOWAH));
		component->effect = aw;
		component->type = AutoWah;
		component->main_effect = 1;
		component->parameters = makeBlankParameters(5, component->effect);
		char temp[480];
		if (strParameters == 0) {
			char *elements =
					"Up Sweep:C*0\tInputGain:S2*0.01,1,10\tFxGain:S2*0.01,1,10\tMin Cutoff Freq:S0*10,267,500\tMax Cutoff Freq:S0*510,2000,3000//Attack Time (s):S3*0.001,0.001,0.01\tRelease Time (s):S2*0.01,0.4,2.0//Q:S2*0.7,4,10\tCutoff Freq:X*2000\tPass:I*1";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		char *ptrParameters = strtok(temp, "//");
		char *ptrFollower = strtok(NULL, "//");
		char *ptrVarBandPass = strtok(NULL, "//");
		// 4 parameters
		component->parameterCount = 5;
		char *params[5];
		char *ptr = strtok(ptrParameters, "\t");
		for (int i = 0; i < component->parameterCount; i++) {
			params[i] = ptr;
			ptr = strtok(NULL, "\t");
		}
		float values[12];
		for (int i = 0; i < component->parameterCount; i++) {
			values[i] = setName_Type_Parse_Variables(component, i, params[i]);
		}
		component->parameters[0].currentValue = (float *)&(aw->down_scan);
		*(component->parameters[0].currentValue) = values[0];
		component->parameters[1].currentValue = &(aw->inputGain);
		*(component->parameters[1].currentValue) = values[1];
		component->parameters[2].currentValue = &(aw->fxGain);
		*(component->parameters[2].currentValue) = values[2];
		component->parameters[3].currentValue = &(aw->minCoFreq);
		*(component->parameters[3].currentValue) = values[3];
		component->parameters[4].currentValue = &(aw->maxCoFreq);
		*(component->parameters[4].currentValue) = values[4];

		// children
		component->childrenCount = 2;
		component->childComponents[0] = createComponent("Variable BandPass",
				ptrVarBandPass, &(aw->vbf));
		component->childComponents[1] = createComponent("Envelope Follower", ptrFollower,
				&(aw->ef));
		component->apply = (APPLY) apply_AUTOWAH;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Equalizer") == 0) {
		EQUALIZER *eq = (EQUALIZER*) MALLOC(sizeof(EQUALIZER));
		component->effect = eq;
		component->type = Equalizer;
		eq->filterCount = 10;
		eq->inv_Count = 1.0f / 10.0f;
		component->parameterCount = 1;
		uint8_t index = 0;

		char temp[15];
		strcpy(temp, "Series:C*0");
		component->parameters = makeBlankParameters(1, component->effect);
		float value = setName_Type_Parse_Variables(component, index, temp);
		component->parameters[index].currentValue = (float *)&(eq->parallel);
		*(component->parameters[index].currentValue) = value + 0.1;
		
		char *bands[5];
		bands[0] =
				"Freq:X*31\tQ:X*4\t31 Hz (db):S2*-20,0,20\nFreq:X*62\tQ:X*4\t62 Hz (db):S2*-20,0,20";
		bands[1] =
				"Freq:X*125\tQ:X*4\t125 Hz (db):S2*-20,0,20\nFreq:X*250\tQ:X*4\t250 Hz (db):S2*-20,0,20";
		bands[2] =
				"Freq:X*500\tQ:X*4\t500 Hz (db):S2*-20,0,20\nFreq:X*1000\tQ:X*4\t1k Hz (db):S2*-20,0,20";
		bands[3] =
				"Freq:X*2000\tQ:X*4\t2k Hz (db):S2*-20,0,20\nFreq:X*4000\tQ:X*4\t4k Hz (db):S2*-20,0,20";
		bands[4] =
				"Freq:X*8000\tQ:X*4\t8k Hz (db):S2*-20,0,20\nFreq:X*16000\tQ:X*4\t16k Hz (db):S2*-20,0,20";
		for (int i = 0; i < 5; ++i) {
			char temp[80];
			strcpy(temp, bands[i]);
			char *ptr1 = strtok(temp, "\n");
			char *ptr2 = strtok(NULL, "\n");
			component->childComponents[2 * i] = createComponent(
					"Equalizing Filter", ptr1, &(eq->filter_band[2 * i]));
			component->childComponents[2 * i + 1] = createComponent(
					"Equalizing Filter", ptr2, &(eq->filter_band[2 * i + 1]));
		}
		component->childrenCount = eq->filterCount;
		component->apply = (APPLY) apply_EQUALIZER;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Chorus") == 0) {
		CHORUS *chorus = (CHORUS*) MALLOC(sizeof(CHORUS));
		component->effect = chorus;
		component->type = Chorus;
		component->parameterCount = 0;
		component->childrenCount = 0;
		component->uninitialize = (UNINITIALIZE) uninitialize_CHORUS;
		char temp[480];
		// forced order: base delay, then Lfo, then Lfo Driven Delay
		if (strParameters == 0) {
			char *elements =
					"Count:I*2//BaseDelay 1:S2*0,1,10\nLFO 1 Freq:S2*0.1,1,5\tLFO 1 Depth (mSec):S2*0,1,10\nDelay 1 Max:X*30//BaseDelay 2:S2*0,1,10\nLFO 2 Freq:S2*0.1,1,5\tLFO 2 Depth (mSec):S2*0,1,10\nDelay 2 Max:X*30";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		char *ptr = temp;
		while (*ptr != '/' && *ptr != 0)
			ptr++;
		if (*(ptr + 1) != '/') {
			component->effect = 0;
			freeComponent(component);
			return 0;
		}
		*ptr = 0;
		char *ptrChorusElements = ptr + 2;
		// time to get our count
		ptr = temp;
		while (*ptr != '*' && *ptr != 0)
			ptr++;
		if (*ptr == 0 || !isdigit(*(ptr + 1))) {
			component->effect = 0;
			freeComponent(component);
			return 0;
		}
		int count = atoi(ptr + 1);
		chorus->chorus_count = count;
		chorus->inv_count = 1.0f / count;
		int componentCount = chorus->chorus_count;
		char *child[12];  // max allowed childen;
		child[0] = strtok(ptrChorusElements, "//");
		for (int i = 1; i < componentCount; ++i) {
			child[i] = strtok(NULL, "//");
		}
		for (int i = 0; i < componentCount; ++i) {
			EFFECT_COMPONENT *cmp = createComponent("Chorus Element", child[i],
					chorus->cElement + i);
			component->childComponents[i] = cmp;
		}

		component->childComponents[componentCount++] = createComponent("Mixer",
				0, &(chorus->mixer));
		component->childrenCount = componentCount;
		component->apply = (APPLY) update_CHORUS;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Compressor") == 0) {
		COMPRESSOR *dr = (COMPRESSOR*) MALLOC(sizeof(COMPRESSOR));
		component->effect = dr;
		component->type = Compressor;
		component->parameterCount = 8;
		component->parameters = makeBlankParameters(8, component->effect);
		component->childrenCount = 0;
		char temp[180];
		// forced order: base delay, then Lfo, then Lfo Driven Delay
		if (strParameters == 0) {
			char *elements =
					"Threshold (db):S2*-50,-10,0\tRatio:S2*1,5,50\tAttack Time (sec):S2*0,0.05,4\tRelease Time (sec):S2*0,0.20,4\tHard Knee:C*1\tKnee Width (db):S2*0,0,20\tMakeupMode:C*0\tMakeup Gain:S2*-10,0,24";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		char *ptrThreshold = strtok(temp, "\t");
		char *ptrRatio = strtok(NULL, "\t");
		char *ptrAttack = strtok(NULL, "\t");
		char *ptrRelease = strtok(NULL, "\t");
		char *ptrHardKnee = strtok(NULL, "\t");
		char *ptrKnee = strtok(NULL, "\t");
		char *ptrMode = strtok(NULL, "\t");
		char *ptrMGain = strtok(NULL, "\t");
		uint8_t index = 0;

		float value = setName_Type_Parse_Variables(component, index,
				ptrThreshold);
		component->parameters[index].currentValue = &(dr->threshold);
		*(component->parameters[index].currentValue) = value;
		index++;
		value = setName_Type_Parse_Variables(component, index,
				ptrRatio);
		component->parameters[index].currentValue = &(dr->gui_ratio);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setInverseRatio_COMPRESSOR;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrAttack);
		component->parameters[index].currentValue = &(dr->attack_time);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setAttackRelease_COMPRESSOR;
		component->parameters[index].partnerParameter = component->parameters + index + 1;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrRelease);
		component->parameters[index].currentValue = &(dr->release_time);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setAttackRelease_COMPRESSOR;
		component->parameters[index].partnerParameter = component->parameters + index - 1;
		index++;
		value = setName_Type_Parse_Variables(component, index,
				ptrHardKnee);
		component->parameters[index].currentValue = (float *)&(dr->gui_not_hard_knee);
		dr->hard_knee = value + 0.1;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setHardKnee_COMPRESSOR;
		index++;
		value = setName_Type_Parse_Variables(component, index,
				ptrKnee);
		component->parameters[index].currentValue = &(dr->knee);
		*(component->parameters[index].currentValue) = value;
		index++;
		value = setName_Type_Parse_Variables(component, index,
				ptrMode);
		component->parameters[index].currentValue = (float *)&(dr->gui_not_hard_property);
		dr->makeup_property_mode = value + 0.1;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setMakeupPropertyKnee_COMPRESSOR;
		index++;
		value = setName_Type_Parse_Variables(component, index,
				ptrMGain);
		component->parameters[index].currentValue = &(dr->makeup_gain);
		*(component->parameters[index].currentValue) = value;
		component->childrenCount = 0;
		component->apply = (APPLY) update_COMPRESSOR;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Limiter") == 0) {
		LIMITER *dr = (LIMITER*) MALLOC(sizeof(LIMITER));
		component->effect = dr;
		component->type = Limiter;
		component->parameterCount = 6;
		component->parameters = makeBlankParameters(6, component->effect);
		component->childrenCount = 0;
		char temp[180];
		// forced order: base delay, then Lfo, then Lfo Driven Delay
		if (strParameters == 0) {
			char *elements =
					"Threshold (db):S2*-50,-10,0\tAttack Time (sec):S2*0,0.0,4\tRelease Time (sec):S2*0,0.20,4\tKnee Width (db):S2*0,0,20\tMakeupMode:C*1\tMakeup Gain:S2*-10,0,24";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		char *ptrThreshold = strtok(temp, "\t");
		char *ptrAttack = strtok(NULL, "\t");
		char *ptrRelease = strtok(NULL, "\t");
		char *ptrKnee = strtok(NULL, "\t");
		char *ptrMode = strtok(NULL, "\t");
		char *ptrMGain = strtok(NULL, "\t");
		uint8_t index = 0;

		float value = setName_Type_Parse_Variables(component, index,
				ptrThreshold);
		component->parameters[index].currentValue = &(dr->threshold_db);
		*(component->parameters[index].currentValue) = value;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrAttack);
		component->parameters[index].currentValue = &(dr->attack_time);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setAttackRelease_LIMITER;
		component->parameters[index].partnerParameter = component->parameters + index + 1;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrRelease);
		component->parameters[index].currentValue = &(dr->release_time);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setAttackRelease_LIMITER;
		component->parameters[index].partnerParameter = component->parameters + index - 1;
		index++;
		value = setName_Type_Parse_Variables(component, index,
				ptrKnee);
		component->parameters[index].currentValue = &(dr->knee);
		*(component->parameters[index].currentValue) = value;
		index++;
		value = setName_Type_Parse_Variables(component, index,
				ptrMode);
		component->parameters[index].currentValue = (float *)&(dr->gui_not_hard_property);
		dr->makeup_property_mode = value + 0.1;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setMakeupPropertyKnee_LIMITER;
		index++;
		value = setName_Type_Parse_Variables(component, index,
				ptrMGain);
		component->parameters[index].currentValue = &(dr->makeup_gain);
		*(component->parameters[index].currentValue) = value;
		component->childrenCount = 0;
		component->apply = (APPLY) update_LIMITER;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Expander") == 0) {
		EXPANDER *dr = (EXPANDER*) MALLOC(sizeof(EXPANDER));
		component->effect = dr;
		component->type = Expander;
		component->parameterCount = 6;
		component->parameters = makeBlankParameters(6, component->effect);
		component->childrenCount = 0;
		char temp[180];
		// forced order: base delay, then Lfo, then Lfo Driven Delay
		if (strParameters == 0) {
			char *elements =
					"Threshold (db):S2*-140,-10,0\tRatio:S2*1,5,50\tHardKnee:C*1\tKnee Width (db):S2*0,0,20\tAttack Time (sec):S2*0,0.05,4\tRelease Time (sec):S2*0,0.20,4\tHold Time:S2*0,0.5,4";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		char *ptrThreshold = strtok(temp, "\t");
		char *ptrRatio = strtok(NULL, "\t");
		char *ptrHardKnee = strtok(NULL, "\t");
		char *ptrKnee = strtok(NULL, "\t");
		char *ptrAttack = strtok(NULL, "\t");
		char *ptrRelease = strtok(NULL, "\t");
		uint8_t index = 0;

		float value = setName_Type_Parse_Variables(component, index,
				ptrThreshold);
		component->parameters[index].currentValue = &(dr->threshold);
		*(component->parameters[index].currentValue) = value;
		index++;
		value = setName_Type_Parse_Variables(component, index,
				ptrRatio);
		component->parameters[index].currentValue = &(dr->ratio);
		*(component->parameters[index].currentValue) = value;
		index++;
		value = setName_Type_Parse_Variables(component, index,
				ptrHardKnee);
		component->parameters[index].currentValue = (float *)&(dr->gui_not_hard_knee);
		dr->hard_knee = value + 0.1;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setHardKnee_EXPANDER;
		index++;
		value = setName_Type_Parse_Variables(component, index,
				ptrKnee);
		component->parameters[index].currentValue = &(dr->knee);
		*(component->parameters[index].currentValue) = value;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrAttack);
		component->parameters[index].currentValue = &(dr->attack_time);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setAttackRelease_EXPANDER;
		component->parameters[index].partnerParameter = component->parameters + index + 1;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrRelease);
		component->parameters[index].currentValue = &(dr->release_time);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setAttackRelease_EXPANDER;
		component->parameters[index].partnerParameter = component->parameters + index - 1;
		component->childrenCount = 0;
		component->apply = (APPLY) update_EXPANDER;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Echo") == 0) {
		component->type = Echo;
		ECHO *echo = (ECHO*) MALLOC(sizeof(ECHO));
		component->effect = echo;
		component->uninitialize = (UNINITIALIZE) uninitialize_ECHO;
		component->parameterCount = 2;
		component->main_effect = 1;
		component->parameters = makeBlankParameters(2, component->effect);
		int index = 0;
		setName_Type(component, 0, "Feedback Gain:S3");
		component->parameters[0].floatParameter[index++] = 0.0f;
		component->parameters[0].floatParameter[index++] = 0.4f;
		component->parameters[0].floatParameter[index++] = 1.0f;
		component->parameters[0].currentValue = &(echo->feedback_gain);

		index = 0;
		setName_Type(component, 1, "Feedback Level:S3");
		component->parameters[1].floatParameter[index++] = 0.0f;
		component->parameters[1].floatParameter[index++] = 0.35f;
		component->parameters[1].floatParameter[index++] = 0.5f;
		component->parameters[1].currentValue = &(echo->feedback_level);
		component->childrenCount = 2;
		index = 0;
		component->childComponents[index++] = createComponent("Mixer", 0,
				&(echo->mixer));
		component->childComponents[index++] = createComponent("Variable Delay",
				"Delay mSec:S2*0,250,500", &(echo->vDelay));
		component->apply = (APPLY) update_Echo;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Flanger") == 0) {
		component->type = Flanger;
		FLANGER *flng = (FLANGER*) MALLOC(sizeof(FLANGER));
		component->effect = flng;
		component->main_effect = 1;
		//component->uninitialize = (UNINITIALIZE) uninitialize_FLANGER;
		component->parameterCount = 2;
		component->parameters = makeBlankParameters(2, component->effect);
		int index = 0;
		setName_Type(component, 0, "Base Delay MSec:S3");
		component->parameters[0].floatParameter[index++] = 0.1f;
		component->parameters[0].floatParameter[index++] = 3.0f;
		component->parameters[0].floatParameter[index++] = 10.0f;
		component->parameters[0].currentValue = &(flng->baseDelay_MSec);
		index = 0;
		setName_Type(component, 1, "Feedback Level:S3");
		component->parameters[1].floatParameter[index++] = 0.0f;
		component->parameters[1].floatParameter[index++] = 0.35f;
		component->parameters[1].floatParameter[index++] = 0.98f;
		component->parameters[1].currentValue = &(flng->feedback_level);
		component->childrenCount = 3;
		index = 0;
		component->childComponents[index++] = createComponent("Mixer", 0,
				&(flng->mixer));
		component->childComponents[index++] = createComponent("Variable Delay",
				"Delay mSec:X*30", &(flng->vDelay));
		component->childComponents[index++] = createComponent("Lfo",
				"LFO Freq:S2*0.1,1,5\tLFO Depth (mSec):S2*0,1,10", 0);
		component->apply = (APPLY) update_FLANGER;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Freeverb") == 0) {
		FREEVERB *fv = (FREEVERB*) MALLOC(sizeof(FREEVERB));
		component->effect = fv;
		component->type = Freeverb;
		component->main_effect = 1;
		component->parameterCount = 0;
		component->childComponents[0] = createComponent("Mixer", 0,
				&(fv->mixer));
		component->childrenCount = 1;
		component->apply = (APPLY) applyFreeverb;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Moorer Reverb") == 0) {
		MOORER_REVERB *fv = (MOORER_REVERB*) MALLOC(sizeof(MOORER_REVERB));
		component->effect = fv;
		component->type = Moorer;
		component->main_effect = 1;
		component->parameterCount = 0;
		component->childComponents[0] = createComponent("Mixer", 0,
				&(fv->mixer));
		component->childrenCount = 1;
		component->apply = (APPLY) apply_MOORER_REVERB;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Noise Gate") == 0) {
		NOISEGATE *ng = (NOISEGATE*) MALLOC(sizeof(NOISEGATE));
		component->effect = ng;
		component->type = Noisegate;
		component->main_effect = 1;
		component->parameters = makeBlankParameters(4, component->effect);
		component->parameterCount = 4;

		char temp[480];
		if (strParameters == 0) {
			char *elements =
					"Threshold(db):S2*-140,-10, 0.0\tHold Time mSec:S2*0,50,4000\tAttack Time mSec:S2*0,50,4000\tRelease Time mSec:S2*0,50,4000";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		char *ptrThreshold = strtok(temp, "\t");
		char *ptrHold = strtok(NULL, "\t");
		char *ptrAttack = strtok(NULL, "\t");
		char *ptrRelease = strtok(NULL, "\t");
		
		uint8_t index = 0;

		float value = setName_Type_Parse_Variables(component, index,
				ptrThreshold);
		component->parameters[index].currentValue = &(ng->gui_thresholdDB);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setNoiseThreshold;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrHold);
		component->parameters[index].currentValue = &(ng->holdTimeMs);
		*(component->parameters[index].currentValue) = value;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrAttack);
		component->parameters[index].currentValue = &(ng->gui_attackMS);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setNoiseAttackRelease;
		component->parameters[index].partnerParameter = component->parameters + index + 1;

		index++;
		value = setName_Type_Parse_Variables(component, index, ptrRelease);
		component->parameters[index].currentValue = &(ng->gui_releaseMS);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setNoiseAttackRelease;
		component->parameters[index].partnerParameter = component->parameters + index - 1;
		component->childrenCount = 0;
		component->apply = (APPLY) applyNoiseGate;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Overdrive") == 0) {
		OVERDRIVE *od = (OVERDRIVE*) MALLOC(sizeof(OVERDRIVE));
		component->effect = od;
		component->type = Overdrive;
		component->main_effect = 1;
		component->parameters = makeBlankParameters(4, component->effect);
		char temp[480];
		if (strParameters == 0) {
			char *elements =
					"PreGain:S2*0.01,1,10\tLow Pass Cutoff Freq:S2*2500,3000,10000\tDamping:X*1.0\tHigh Pass Cutoff Freq:S2*2000,2000,3000";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		char *ptrGain = strtok(temp, "\t");
		char *ptrLPFFreq = strtok(NULL, "\t");
		char *ptrDamp = strtok(NULL, "\t");
		char *ptrHPFFreq = strtok(NULL, "\t");
		
		uint8_t index = 0;
		float value = setName_Type_Parse_Variables(component, index, ptrGain);
		component->parameters[index].currentValue = &(od->preGain);
		*(component->parameters[index].currentValue) = value;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrLPFFreq);
		component->parameters[index].currentValue = &(od->gui_LPFFreq);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_overdriveSetLPF;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrDamp);
		component->parameters[index].currentValue = &(od->lpfOutDamp);
		*(component->parameters[index].currentValue) = value;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrHPFFreq);
		component->parameters[index].currentValue = &(od->gui_HPFFreq);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_overdriveSetHPF;
		component->parameterCount = 4;
		component->childrenCount = 0;
		component->apply = (APPLY) update_OVERDRIVE;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Schroeder Reverb") == 0) {
		SCHROEDERVERB *fv = (SCHROEDERVERB*) MALLOC(sizeof(SCHROEDERVERB));
		component->effect = fv;
		component->type = Schroeder;
		component->main_effect = 1;
		component->parameterCount = 0;
		component->childComponents[0] = createComponent("Mixer", 0,
				&(fv->mixer));
		component->childrenCount = 1;
		component->apply = (APPLY) applyShroederVerb;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Tremolo") == 0) {
		TREMOLO *trem = (TREMOLO*) MALLOC(sizeof(TREMOLO));
		component->effect = trem;
		component->type = Tremolo;
		component->main_effect = 1;
		component->parameters = makeBlankParameters(1, component->effect);
		component->parameterCount = 1;
		char temp[480];
		if (strParameters == 0) {
			char *elements =
					"Depth:S2*0.0,0.5,1.0//LFO Freq:S2*0.01,2000,4000\tDepth:X*1.0\tSine,Triangle:R*0";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		char *ptrDepth = strtok(temp, "//");
		char *ptrLfo = strtok(NULL, "//");
		float value = setName_Type_Parse_Variables(component, 0, ptrDepth);
		component->parameters[0].currentValue = &(trem->depth);
		*(component->parameters[0].currentValue) = value;
		uint8_t index = 0;
		component->childComponents[index++] = createComponent("Lfo", ptrLfo,
				&(trem->osc));
		component->childComponents[index++] = createComponent("Mixer", 0,
				&(trem->mixer));
		component->childrenCount = index;
		component->apply = (APPLY) gui_update_TREMOLO;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Vibrato") == 0) {
		component->type = Vibrato;
		VIBRATO *vib = (VIBRATO*) MALLOC(sizeof(VIBRATO));
		component->effect = vib;
		component->main_effect = 1;
		component->uninitialize = (UNINITIALIZE) uninitialize_VIBRATO;
		char temp[160];
		if (strParameters == 0) {
			char *elements =
					"Base Delay (MS):S2*0.1,5,14//Max Delay:X*15//LFO Frequency:S2*0.1,5,15\tLFO Depth (mSec):S2*0,2.5,5";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		char *ptrBaseDelay = strtok(temp, "//");
		char *ptrVarDelay = strtok(NULL, "//");
		char *ptrLFO = strtok(NULL, "//");

		component->parameterCount = 1;
		component->parameters = makeBlankParameters(1, component->effect);
		float value = setName_Type_Parse_Variables(component, 0, ptrBaseDelay);
		component->parameters[0].currentValue = &(vib->baseDelayMSec);
		*(component->parameters[0].currentValue) = value;
		component->childrenCount = 3;
		component->childComponents[0] = createComponent("Lfo", ptrLFO,
				&(vib->lfo));
		component->childComponents[1] = createComponent("Variable Delay",
				ptrVarDelay, &(vib->vDelay));
		component->childComponents[2] = createComponent("Mixer", 0,
				&(vib->mixer));
		component->apply = (APPLY) apply_VIBRATO;
		component->effect_bypass = 0;

	} else if (strcmp(effectName, "Wah Wah") == 0) {
		component->type = WahWah;
		component->main_effect = 1;
		char temp[160];
		if (strParameters == 0) {
			char *elements = "Q:X*6\tCenter Frequency:S2*500,2000,5000"
					"//LFO Freq:S2*0.1,2000,3000\tLFO Depth (Hz):S2*0,250,1000";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		component->parameterCount = 1;
		component->parameters = makeBlankParameters(1, component->effect);
		char *ptr = strtok(temp, "*");
		setName_Type(component, 0, ptr);
		while (*ptr != ':' && *ptr != 0)
			ptr++;
		if (*ptr == ':' && *(ptr + 1) == 'S') {
			//uint8_t count = atoi(ptr + 2);
			uint8_t count = 3;
			ptr = strtok(NULL, "*");
			ptr = parseParameters(ptr, component->parameters, count);
		} else {
			freeComponent(component);
		}
	} else if (strcmp(effectName, "Chorus Element") == 0) {
		CHORUSELEMENT *cElement = (CHORUSELEMENT*) effect;
		if (effect == 0) {
			cElement = (CHORUSELEMENT*) MALLOC(sizeof(CHORUSELEMENT));
			component->main_effect = 1;
		}
		component->effect = cElement;
		component->type = ChorusElement;
		char temp[160];
		strcpy(temp, strParameters);
		// forced order: base delay, then Lfo, then Lfo Driven Delay
		char *ptrBaseDelay = strtok(temp, "\n");  // baseDelay
		char *ptrLfo = strtok(NULL, "\n");  // LFO
		char *ptrVarDelay = strtok(NULL, "\n");  // Driven VARDELAY
		component->parameterCount = 1;
		component->parameters = makeBlankParameters(1, component->effect);
		float value = setName_Type_Parse_Variables(component, 0, ptrBaseDelay);
		component->parameters[0].currentValue = &(cElement->baseDelayMSec);
		*(component->parameters[0].currentValue) = value;
		component->childrenCount = 2;
		component->childComponents[0] = createComponent("Lfo", ptrLfo,
				&(cElement->lfo));
		component->childComponents[1] = createComponent("Variable Delay",
				ptrVarDelay, &(cElement->vDelay));
		component->apply = 0;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Envelope Follower") == 0) {
		component->type = EnvelopeFollower;
		ENVELOPE_FOLLOWER *ef = (ENVELOPE_FOLLOWER*) effect;
		if (effect == 0) {
			ef = (ENVELOPE_FOLLOWER*) MALLOC(sizeof(ENVELOPE_FOLLOWER));
			component->main_effect = 1;
		}
		component->effect = ef;
		component->parameterCount = 2;
		component->parameters = makeBlankParameters(2, component->effect);
		uint8_t index = 0;
		char temp[160];
		strcpy(temp, strParameters);
		char *ptrAttack = strtok(temp, "\t");
		char *ptrRelease = strtok(NULL, "\t");
		float value = setName_Type_Parse_Variables(component, index, ptrAttack);
		component->parameters[index].currentValue = &(ef->gui_attack_time);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate = (RECALCULATE)gui_setAttackRelease_ENVELOPE_FOLLOWER;
		component->parameters[index].partnerParameter = &(component->parameters[index + 1]);
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrRelease);
		component->parameters[index].currentValue = &(ef->gui_release_time);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate = (RECALCULATE)gui_setAttackRelease_ENVELOPE_FOLLOWER;
		component->parameters[index].partnerParameter = &(component->parameters[index - 1]);

		component->childrenCount = 0;
		component->apply = 0;
	} else if (strcmp(effectName, "Equalizing Filter") == 0) {
		component->type = EqualizingFilter;
		EQFILTER *ef = (EQFILTER*) effect;
		if (effect == 0) {
			ef = (EQFILTER*) MALLOC(sizeof(EQFILTER));
			component->main_effect = 1;
		}
		component->effect = ef;
		char temp[160];
		strcpy(temp, strParameters);
		char *ptrFreq = strtok(temp, "\t");
		char *ptrQ = strtok(NULL, "\t");
		char *ptrGain = strtok(NULL, "\t");
		component->parameterCount = 3;
		uint8_t index = 0;
		component->parameters = makeBlankParameters(3, component->effect);
		float value = setName_Type_Parse_Variables(component, index, ptrFreq);
		ef->gui_freq = value;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrQ);
		ef->Q = value;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrGain);
		component->parameters[index].currentValue = &(ef->gui_gainDB);
		*(component->parameters[index].currentValue) = value;
		component->parameters[index].recalculate =
				(RECALCULATE) gui_setGain_EQFILTER;
		component->childrenCount = 0;
		component->apply = 0;
	} else if (strcmp(effectName, "Lfo") == 0) {
		component->type = Lfo;
		LOWFREQOSC *lfo = (LOWFREQOSC*) effect;
		if (effect == 0) {
			lfo = (LOWFREQOSC*) MALLOC(sizeof(LOWFREQOSC));
			component->main_effect = 1;
		}
		component->effect = lfo;
		// must have 2 parameters frequency and Depth
		char temp[80];
		int sine = 0;
		int phase = 0;
		strcpy(temp, strParameters);
		char *ptrFreq = strtok(temp, "\t");
		char *ptrDepth = strtok(NULL, "\t");
		char *ptrSine = strtok(NULL, "\t");
		component->parameters = makeBlankParameters(4, component->effect); // may need 4 for sine and phase
		component->parameterCount = 2;
		float value = setName_Type_Parse_Variables(component, 0, ptrFreq);
		component->parameters[0].currentValue = &(lfo->oscFreq);
		*(component->parameters[0].currentValue) = value;
		component->parameters[0].recalculate = (RECALCULATE) gui_setFrequency;
		value = setName_Type_Parse_Variables(component, 1, ptrDepth);
		component->parameters[1].currentValue = &(lfo->amplitude);
		*(component->parameters[1].currentValue) = value;
		if (ptrSine) {
			value = setName_Type_Parse_Variables(component, 2, ptrSine);
			sine = value + 0.1f;
			component->parameters[2].recalculate = (RECALCULATE) gui_setOscType;
			component->parameters[2].currentValue = (float *) &(lfo->triangle_sine_select);
			*(component->parameters[2].currentValue) = sine;
			component->parameterCount = 3;

		}
		lfo->triangle_sine_select = sine;
		lfo->phaseAngleDeg = phase;

		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Mixer") == 0) {
		component->type = Mixer;
		MIXER *mixer = (MIXER*) effect;
		if (effect == 0) {
			mixer = (MIXER*) malloc(sizeof(MIXER));
			component->main_effect = 1;
		}
		component->effect = mixer;
		component->type = Mixer;
		component->parameterCount = 1;
		component->parameters = makeBlankParameters(1, component->effect);
		int index = 0;
		setName_Type(component, 0, "Wet/Dry:S3");
		component->parameters->floatParameter[index++] = 0.0f;
		component->parameters->floatParameter[index++] = 1.0f;
		component->parameters->floatParameter[index++] = 1.0f;
		component->parameters[0].currentValue = &(mixer->wet_dry);
		mixer->wet_dry = 0.96f;
		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "State Variable Filter") == 0) {
		component->type = StateVariableFilter;
		SVFILTER *svf = (SVFILTER*) effect;
		if (effect == 0) {
			svf = (SVFILTER*) MALLOC(sizeof(SVFILTER));
			component->main_effect = 1;
		}
		component->effect = svf;
		char temp[160];
		if (strParameters == 0) {
			char *elements =
				"Damping:S2*0.01, 0.05,1.0\tCenter Frequency:S2*500,2000,5000";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		component->parameterCount = 2;
		component->parameters = makeBlankParameters(2, component->effect);
		char *ptrDamping = strtok(temp, "\t");
		char *ptrFreq = strtok(NULL, "\t");
		float value = setName_Type_Parse_Variables(component, 0, ptrDamping);
		component->parameters[0].currentValue = &(svf->damping);
		*(component->parameters[0].currentValue) = value;
		component->parameters[0].recalculate =
			(RECALCULATE) gui_set_frequency_damping_SVFILTER;
		component->parameters[0].partnerParameter = &(component->parameters[1]);
		value = setName_Type_Parse_Variables(component, 0, ptrFreq);
		component->parameters[1].currentValue = &(svf->centerFreq);
		*(component->parameters[1].currentValue) = value;
		component->parameters[1].recalculate =
			(RECALCULATE) gui_set_frequency_damping_SVFILTER;
		component->parameters[1].partnerParameter = &(component->parameters[0]);
		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;

	} else if (strcmp(effectName, "Variable BandPass") == 0) {
		component->type = VariableBandpass;

		VARBANDPASS *vbp = (VARBANDPASS*) effect;
		if (effect == 0) {
			vbp = (VARBANDPASS*) MALLOC(sizeof(VARBANDPASS));
			component->main_effect = 1;
		}
		component->effect = vbp;
		char temp[80];
		strcpy(temp, strParameters);
		char *ptrQ = strtok(temp, "\t");
		char *ptrCutoff = strtok(NULL, "\t");
		char *ptrPassBlock = strtok(NULL, "\t");
		component->parameterCount = 3;
		component->parameters = makeBlankParameters(3, component->effect);
		// three parameters
		float value = setName_Type_Parse_Variables(component, 0, ptrQ);
		component->parameters[0].currentValue = &(vbp->gui_Q);
		*(component->parameters[0].currentValue) = value;
		//component->parameters[0].recalculate = (RECALCULATE)gui_setCenterFrequency_VARBANDPASS;
		//component->parameters[0].partnerParameter = &(component->parameters[1]); 
		value = setName_Type_Parse_Variables(component, 1, ptrCutoff);
		component->parameters[1].currentValue = &(vbp->gui_Freq);
		*(component->parameters[1].currentValue) = value;
		//component->parameters[1].recalculate = (RECALCULATE)gui_setCenterFrequency_VARBANDPASS;
		//component->parameters[1].partnerParameter = &(component->parameters[0]); 
		setName_Type_Parse_Variables(component, 2, ptrPassBlock);
		vbp->pass_stop = component->parameters[2].intParameter[0];
		// no children
		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;

	} else if (strcmp(effectName, "Volume") == 0) {
		component->type = Volume;
		VOLUME *vol = effect;
		if (effect == 0) {
			vol = (VOLUME*) MALLOC(sizeof(VOLUME));
			component->main_effect = 1;
		}
		component->effect = vol;
		component->type = Volume;
		char temp[80];
		if (strParameters == 0) {
			char *elements = "MaxGain:X*1.0\tVolume:S2*0.0,0.9,1.0";
			strcpy(temp, elements);
		} else {
			strcpy(temp, strParameters);
		}
		char *ptrMaxGain = strtok(temp, "\t");
		char *ptrVolume = strtok(NULL, "\t");
		component->parameterCount = 2;
		int index = 0;
		component->parameters = makeBlankParameters(2, component->effect);
		float value = setName_Type_Parse_Variables(component, index,
				ptrMaxGain);
		component->parameters[index].currentValue = &(vol->maxVolume);
		*(component->parameters[index].currentValue) = value;
		index++;
		value = setName_Type_Parse_Variables(component, index, ptrVolume);
		component->parameters[index].currentValue = &(vol->volume);
		*(component->parameters[index].currentValue) = value;
		component->childrenCount = 0;
		component->apply = 0;
		component->effect_bypass = 0;
	} else if (strcmp(effectName, "Variable Delay") == 0) {
		component->type = VariableDelay;
		VARDELAY *vDelay = (VARDELAY*) effect;
		if (effect == 0) {
			vDelay = (VARDELAY*) MALLOC(sizeof(VARDELAY));
		}
		component->effect = vDelay;
		vDelay->cBuf.storage = 0;
		char temp[80];
		strcpy(temp, strParameters);
		component->parameterCount = 1;
		component->parameters = makeBlankParameters(1, component->effect);
		float maxDelay = 10.0;
		float current_delay = setName_Type_Parse_Variables(component, 0, temp);
		if (component->strTypes[0][0] == 'S') {
			maxDelay = component->parameters[0].floatParameter[2];
		} else {
			maxDelay = component->parameters[0].floatParameter[0];
		}
		component->parameters[0].currentValue = &(vDelay->gui_delayMSec);
		component->parameters[0].recalculate =
			(RECALCULATE) gui_setDelay_VARDELAY;
		*(component->parameters[0].currentValue) = current_delay;
		vDelay->max_delay = maxDelay / 1000.0f;
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
	char *cpy = (char*) MALLOC(strlen(string) + 1);
	strcpy(cpy, string);
	return cpy;
}
#endif
void do_nothing_Component(void *effect) {
	return;
}
