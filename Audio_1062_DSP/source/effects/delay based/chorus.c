/*
 * chorus.c
 *
 *  Created on: Sep 24, 2024
 *      Author: isaac
 */


#include "chorus.h"

void initialize_CHORUS (CHORUS *chorus, uint8_t chorus_count, CHORUSELEMENT * chElement[], float *buf[], float wet_dry,
		uint32_t buf_size, float *freq, float *depth, float delayMSec, float sampleRate) {
	initialize_MIXER (&(chorus->mixer), wet_dry);
	chorus->sampleRate = sampleRate;
	chorus->chorus_count = chorus_count;
	for (int i = 0; i < chorus_count; i++) {
		initialize_CHORUSELEMENT(chElement[i], buf[i], buf_size, delayMSec,
				depth[i], freq[i], sampleRate);
	}
	initialize_MIXER (&(chorus->mixer), wet_dry);
}

void setWetDry_CHORUS (CHORUS *chorus, float wet_dry) {
	setWetDry_MIXER (&(chorus->mixer), wet_dry);
}
void setBaseDelayMSec_CHORUS (CHORUS *chorus, float delayMSec) {
	for (int i=0; i < chorus->chorus_count; ++i) {
		setBaseDelayMS_CHORUSELEMENT(&(chorus->cElement[i]), delayMSec);
	}
	//chorus->baseDelayMsec = chorus->cElement[0].baseDelayMS);
}
void setDepth_CHORUS(CHORUS *chorus, float depth) {
	for (int i=0; i < chorus->chorus_count; ++i) {
		setAmplitude_LOWFREQOSC(&(chorus->cElement[i].lfo), depth);
	}
}
float update_CHORUS (CHORUS *chorus, float input) {
	chorus->output = 0.0f;
	for (int i=0; i < chorus->chorus_count; ++i) {
		chorus->output += update_CHORUSELEMENT (&(chorus->cElement[i]), input);
	}
	chorus->output = applyWetDry_MIXER (&(chorus->mixer), chorus->output / chorus->chorus_count, input);
	return chorus->output;
}

