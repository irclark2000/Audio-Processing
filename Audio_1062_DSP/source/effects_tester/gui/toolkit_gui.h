/*
 * toolkit_gui.h
 *
 *  Created on: Oct 30, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_TESTER_GUI_TOOLKIT_GUI_H_
#define EFFECTS_TESTER_GUI_TOOLKIT_GUI_H_
#include "effect_component.h"
#include "effects_gui_interface.h"
// effects slider parameters
typedef struct {
	EFFECT_PARAMS *myParameter;
	char *name;
	float slope;
	float intercept;
	float *slOutput;
	float previousOutput;
	float slider_value;  // 0.0->1.0;
} SLIDER_VALUES;


void update_effect_state(SLIDER_VALUES *sliders, uint8_t slider_count);
void generate_gui(EFFECT_ITEM *effects_list, uint8_t count);
void update_state_by_counter (uint16_t counter, uint16_t max_counter);
void update_state_periodically ();
void selected_music_file (char * fileName);


#endif /* EFFECTS_TESTER_GUI_TOOLKIT_GUI_H_ */
