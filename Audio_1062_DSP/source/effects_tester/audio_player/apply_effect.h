/*
 * apply_effect.h
 *
 *  Created on: Oct 17, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_TESTER_AUDIO_PLAYER_APPLY_EFFECT_H_
#define EFFECTS_TESTER_AUDIO_PLAYER_APPLY_EFFECT_H_

typedef struct {
        long intParams[4];
        long floatParms[4];
        void *buffer;
} EFFECT_PARAMS;
void  initializeExternalEffect(void *effect, float sampleRate);
float applyExternalEffectCode (void *effect, float input);
void * getExternalEffect();
EFFECT_PARAMS * getExternalParameters();

#endif /* EFFECTS_TESTER_AUDIO_PLAYER_APPLY_EFFECT_H_ */