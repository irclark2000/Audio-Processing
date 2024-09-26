/*
 * chorus.h
 *
 *  Created on: Sep 24, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_DELAY_BASED_CHORUS_H_
#define EFFECTS_DELAY_BASED_CHORUS_H_

#include "chorus_element.h"
#include "mixer.h"

#define MAX_NUMBER_CHORUSELEMENTS 3
typedef struct {
	CHORUSELEMENT cElement[MAX_NUMBER_CHORUSELEMENTS];
	uint8_t chorus_count;
	float inv_count;   // allows using multiplication rather than division
	float sampleRate;
	float output;
	MIXER mixer;
} CHORUS;

void initialize_CHORUS (CHORUS *chorus, uint8_t chorus_count, float *buf[], float wet_dry,
		uint32_t buf_size, float delayMSec, CELEMENTDATA *cData, float sampleRate);
void setWetDry_CHORUS (CHORUS *chorus, float wet_dry);
void setBaseDelayMSec_CHORUS (CHORUS *chorus, float delayMS);
void setDepth_CHORUS(CHORUS *chorus, float depth, uint8_t element);
void setFrequency_CHORUS(CHORUS *chorus, float freq, uint8_t element);
float update_CHORUS (CHORUS *chorus, float input);


#endif /* EFFECTS_DELAY_BASED_CHORUS_H_ */
