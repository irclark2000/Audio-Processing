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

static void setupSliders(DISPLAY_STATE *gui, EFFECT_COMPONENT *component);

static void clearDisplayState(DISPLAY_STATE *gui) {
	gui->display_sliders = 0;
	gui->effect_selected = 0;
	gui->slider_count = 0;
	if (gui->component) {
		freeComponent(gui->component);
		gui->component = NULL;
	}
}
