/*
 * effect_controls.c
 *
 *  Created on: Nov 26, 2024
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


#include "effect_controls.h"
#include <assert.h>

void update_effect_state_for_slider(SLIDER_VALUES *sliders, uint8_t index) {
	EFFECT_PARAMS *parameter = sliders[index].myParameter;
	void *effect = parameter->myEffect;
	uint8_t useCheckbox = sliders[index].useCheckBox;
	if (useCheckbox) {
		sliders[index].previousCheck = *sliders[index].chkOutput;
		return;
	}
	if (parameter->recalculate) {
		if (parameter->previousValue != 0
				&& (*(parameter->currentValue) != *(parameter->previousValue))) {
			parameter->recalculate(effect);
			*(parameter->previousValue) = *(parameter->currentValue);
			EFFECT_PARAMS *pParameter = parameter->partnerParameter;
			if (pParameter) {
				if (pParameter->previousValue != 0
						&& (*(pParameter->currentValue)
								!= *(pParameter->previousValue))) {
					*(pParameter->previousValue) = *(pParameter->currentValue);
				}

			}
		}
	} else if (parameter->previousValue != 0) { // if we don't use recalc
		*(parameter->previousValue) = *(parameter->currentValue);
	}
}
void update_effect_state(SLIDER_VALUES *sliders, uint8_t slider_count) {
	for (uint8_t index = 0; index < slider_count; ++index) {
		update_effect_state_for_slider(sliders, index);
	}
}

void update_state_periodically() {
	static uint32_t count = 0;
	uint8_t index = count % gGUI.slider_count;
	update_effect_state_for_slider(gGUI.sliders, index);
	if (count++ == 400) {
		count = 0;
	}
}

static void clearDisplayState(DISPLAY_STATE *gui) {
	gui->display_sliders = 0;
	gui->effect_selected = 0;
	gui->slider_count = 0;
	if (gui->component) {
		freeComponent(gui->component);
		gui->component = NULL;
	}
}

#define INITIAL_FLOAT_VALUE -1888.8888f

static void setupSlidersComponent(DISPLAY_STATE *gui, EFFECT_PARAMS *parameter) {
	uint8_t count = gui->slider_count;
	gui->sliders[count].useCheckBox = 0;

	assert(parameter->currentValue != 0);
	gui->sliders[count].myParameter = parameter;
	gui->sliders[count].slope = parameter->floatParameter[2]
			- parameter->floatParameter[0];
	gui->sliders[count].intercept = parameter->floatParameter[0];
	gui->sliders[count].slider_value = (parameter->floatParameter[1]
			- parameter->floatParameter[0]) / gui->sliders[count].slope;
	gui->sliders[count].slOutput = parameter->currentValue;
	*(parameter->currentValue) = parameter->floatParameter[1];
	gui->sliders[count].previousOutput = INITIAL_FLOAT_VALUE;
	parameter->previousValue = &(gui->sliders[count].previousOutput);
}

void setupSliders(DISPLAY_STATE *gui, EFFECT_COMPONENT *component) {
	for (int i = 0; i < component->parameterCount; i++) {
		char *name = component->strParameters[i];
		if (component->strTypes[i][0] == 'S') {
			EFFECT_PARAMS *parameter = component->parameters + i;
			setupSlidersComponent(gui, parameter);
			gui->sliders[gui->slider_count].name = name;
			gui->slider_count++;
		} else if (component->strTypes[i][0] == 'C') {
			EFFECT_PARAMS *parameter = component->parameters + i;
			uint8_t count = gui->slider_count;
			gui->sliders[count].myParameter = parameter;
			gui->sliders[count].useCheckBox = 1;
			gui->sliders[count].name = name;
			gui->sliders[count].chkOutput = (int*) parameter->currentValue;
			*(gui->sliders[count].chkOutput) = parameter->intParameter[0] & 0xFF;
			gui->sliders[count].previousCheck = 15;
			gui->slider_count++;
		}
	}
	for (int i = 0; i < component->childrenCount; i++) {
		EFFECT_COMPONENT *childComponent = component->childComponents[i];
		setupSliders(gui, childComponent);
	}
}
