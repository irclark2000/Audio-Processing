/*
 * effects_gui_interface.h
 *
 *  Created on: Oct 24, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_TESTER_GUI_EFFECTS_GUI_INTERFACE_H_
#define EFFECTS_TESTER_GUI_EFFECTS_GUI_INTERFACE_H_
#include <variable_filter_effects/wah_wah.h>
#include "effects_tester/audio_player/effect_component.h"
#include "effects/delay_based/echo.h"
#include "effects/delay_based/chorus.h"
#include "effects/delay_based/flanger.h"
#include "effects/delay_based/vibrato.h"

typedef struct {
	char *name;
	EFFECT_TYPE eType;
} EFFECT_ITEM;

extern EFFECT_ITEM effects_list[];
void initializeEffect(void *type, EFFECT_COMPONENT *effect, float sampleRate);
float applyEffect_COMPONENT(void *type, EFFECT_COMPONENT *effect, float input);
void setParameter_COMPONENT(void *type, EFFECT_COMPONENT *effect, char *pName, float value);
float getParameter_COMPONENT(void *type, EFFECT_COMPONENT *effect, char *pName);
void unintializeEffect(void * type);

#endif /* EFFECTS_TESTER_GUI_EFFECTS_GUI_INTERFACE_H_ */
