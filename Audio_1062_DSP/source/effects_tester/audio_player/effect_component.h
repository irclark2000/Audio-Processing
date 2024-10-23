/*
 * effect_component.h
 *
 *  Created on: Oct 21, 2024
 *      Author: isaac
 */

#include <stdint.h>
#include "apply_effect.h"
#ifndef EFFECTS_TESTER_AUDIO_PLAYER_EFFECT_COMPONENT_H_
#define EFFECTS_TESTER_AUDIO_PLAYER_EFFECT_COMPONENT_H_

struct EFFECT_COMPONENT;
#define MAX_CHILD_EFFECT_COMPONENTS 10

typedef struct {
	char *name;
	void * effect;
	void (*initialize) (void *, EFFECT_PARAMS *parameters);
	float (*apply) (void *, float);
	void (*uninitialize) (void *);
	char component_parameters[5][80];
	EFFECT_PARAMS *parameters;
	uint8_t parameterCount;
	struct EFFECT_COMPONENT *childComponents[MAX_CHILD_EFFECT_COMPONENTS];
	uint8_t childrenCount;
} EFFECT_COMPONENT;

#endif /* EFFECTS_TESTER_AUDIO_PLAYER_EFFECT_COMPONENT_H_ */
