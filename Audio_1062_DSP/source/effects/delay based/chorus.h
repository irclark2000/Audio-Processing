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
	//float baseDelayMsec;
	float sampleRate;
	float output;
	MIXER mixer;
} CHORUS;

void initialize_CHORUS (CHORUS *chorus, uint8_t chorus_count, CHORUSELEMENT * chElement[], float *buf[], float wet_dry,
		uint32_t buf_size, float *freq, float *depth, float delayMSec, float sampleRate);
void setWetDry_CHORUS (CHORUS *chorus, float wet_dry);
void setDelayMSec_CHORUS (CHORUS *chorus, float delayMS);
float update_CHORUS (CHORUS *chorus, float input);
void setDepth_CHORUS(CHORUS *chorus, float depth);

#endif /* EFFECTS_DELAY_BASED_CHORUS_H_ */
