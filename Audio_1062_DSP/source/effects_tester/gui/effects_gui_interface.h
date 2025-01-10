/*
 * effects_gui_interface.h
 *
 *  Created on: Oct 24, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_TESTER_GUI_EFFECTS_GUI_INTERFACE_H_
#define EFFECTS_TESTER_GUI_EFFECTS_GUI_INTERFACE_H_
//#include "effects_tester/audio_player/effect_component.h"
#include "effects_tester/audio_player/audio_component.h"
#include "effects/delay_based/echo.h"
#include "effects/delay_based/chorus.h"
#include "effects/delay_based/flanger.h"
#include "effects/delay_based/vibrato.h"
#include "effects/variable_filter_effects/auto_wah.h"
#include "effects/variable_filter_effects/wah_wah.h"

typedef struct {
	char *name;
	EFFECT_TYPE eType;
} EFFECT_ITEM;

extern EFFECT_ITEM effects_list[];
void gui_initialize(EFFECT_COMPONENT *ec, uint32_t size, float sampleRate);
void unintializeEffect(void * type);

#endif /* EFFECTS_TESTER_GUI_EFFECTS_GUI_INTERFACE_H_ */
