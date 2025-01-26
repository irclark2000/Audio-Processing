/*
 * effect_controls.h
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

#ifndef EFFECTS_TESTER_GUI_EFFECT_CONTROLS_H_
#define EFFECTS_TESTER_GUI_EFFECT_CONTROLS_H_
#include <stdint.h>
//#include "effects_tester/audio_player/effect_component.h"
#include "effects_tester/audio_player/audio_component.h"
#include "effects_gui_interface.h"

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

#define MAX_SLIDER_COUNT 15

typedef struct {
	int display_sliders;
	int effect_selected;
	int previous_effect;
	float effect_volume;
	int effect_enabled;
	//EFFECT_COMPONENT *component;
	uint8_t slider_count[MAX_NUMBER_CHANNELS];
	SLIDER_VALUES sliders[MAX_NUMBER_CHANNELS][MAX_SLIDER_COUNT];
	AUDIO_COMPONENT channels;
} DISPLAY_STATE;

extern DISPLAY_STATE gGUI;
extern EFFECT_ITEM *g_effect_list;
extern uint8_t g_effects_count;
extern SLIDER_FORMAT gFormats[];

void update_effect_state(SLIDER_VALUES *sliders, uint8_t slider_count);
void generate_gui(EFFECT_ITEM *effects_list);
void update_state_by_counter (uint16_t counter, uint16_t max_counter);
void update_state_periodically ();
void update_effect_state_for_slider(SLIDER_VALUES *sliders, uint8_t index);
void setupSliders(DISPLAY_STATE *gui, EFFECT_COMPONENT *component, uint8_t channel);
void clearDisplayState(DISPLAY_STATE *gui);
void initializeDisplayState(DISPLAY_STATE *gui, uint8_t selection, uint8_t channel);

#endif /* EFFECTS_TESTER_GUI_EFFECT_CONTROLS_H_ */
