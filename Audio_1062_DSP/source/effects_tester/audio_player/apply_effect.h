/*
 * apply_effect.h
 *
 *  Created on: Oct 17, 2024
 *      Author: isaac
 */
#include <stdint.h>

#ifndef EFFECTS_TESTER_AUDIO_PLAYER_APPLY_EFFECT_H_
#define EFFECTS_TESTER_AUDIO_PLAYER_APPLY_EFFECT_H_
#define MAX_FLOAT_PARAMETERS 4
typedef struct {
        uint32_t intParameter[10];
        float floatParameter[MAX_FLOAT_PARAMETERS];
	float *currentValue;
        void *buffer;
} EFFECT_PARAMS;
void  initializeExternalEffect(int index, float sampleRate);
float applyExternalEffectCode (int index, float input);
void * getExternalEffect(int index);
EFFECT_PARAMS * getExternalParameters(int index);

#endif /* EFFECTS_TESTER_AUDIO_PLAYER_APPLY_EFFECT_H_ */
