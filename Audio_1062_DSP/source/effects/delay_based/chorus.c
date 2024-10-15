/*
 * chorus.c
 *
 *  Created on: Sep 24, 2024
 *      Author: isaac
 */


#include "chorus.h"

void initialize_CHORUS (CHORUS *chorus, uint8_t chorus_count, float *buf[], float wet_dry,
		uint32_t buf_size, float delayMSec, CELEMENTDATA *cData, float sampleRate) {
	initialize_MIXER (&(chorus->mixer), wet_dry);
	chorus->sampleRate = sampleRate;
	chorus->chorus_count = chorus_count;
	chorus->inv_count = 1.0f / chorus_count;
	for (int i = 0; i < chorus_count; i++) {
		initialize_CHORUSELEMENT(&(chorus->cElement[i]), buf[i], buf_size, delayMSec,
				cData[i], sampleRate);
	}
	initialize_MIXER (&(chorus->mixer), wet_dry);
}

void setWetDry_CHORUS (CHORUS *chorus, float wet_dry) {
	setWetDry_MIXER (&(chorus->mixer), wet_dry);
}
// all element have the same delay but different depths and frequencies
void setBaseDelayMSec_CHORUS (CHORUS *chorus, float delayMSec) {
	for (int i=0; i < chorus->chorus_count; ++i) {
		setBaseDelayMSec_CHORUSELEMENT(&(chorus->cElement[i]), delayMSec);
	}
	//chorus->baseDelayMsec = chorus->cElement[0].baseDelayMS);
}
void setDepth_CHORUS(CHORUS *chorus, float depth, uint8_t element) {
	setDepth_CHORUSELEMENT(&(chorus->cElement[element]), depth);
}
void setFrequency_CHORUS(CHORUS *chorus, float freq, uint8_t element) {
	setFrequency_CHORUSELEMENT(&(chorus->cElement[element]), freq);
}

float update_CHORUS (CHORUS *chorus, float input) {
	chorus->output = 0.0f;
	for (int i=0; i < chorus->chorus_count; ++i) {
		chorus->output += update_CHORUSELEMENT (&(chorus->cElement[i]), input);
	}
	chorus->output = applyWetDry_MIXER (&(chorus->mixer), chorus->output * chorus->inv_count, input);
	return chorus->output;
}

