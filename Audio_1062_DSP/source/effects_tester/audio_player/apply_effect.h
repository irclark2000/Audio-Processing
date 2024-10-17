/*
 * apply_effect.h
 *
 *  Created on: Oct 17, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_TESTER_AUDIO_PLAYER_APPLY_EFFECT_H_
#define EFFECTS_TESTER_AUDIO_PLAYER_APPLY_EFFECT_H_

typedef struct {
        long intParameter[10];
        long floatParameter[10];
        void *buffer;
} EFFECT_PARAMS;
void  initializeExternalEffect(int index, float sampleRate);
float applyExternalEffectCode (int index, float input);
void * getExternalEffect(int index);
EFFECT_PARAMS * getExternalParameters(int index);

#endif /* EFFECTS_TESTER_AUDIO_PLAYER_APPLY_EFFECT_H_ */
