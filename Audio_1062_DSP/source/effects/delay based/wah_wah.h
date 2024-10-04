/*
 * wah_wah.h
 *
 *  Created on: Oct 3, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_DELAY_BASED_WAH_WAH_H_
#define EFFECTS_DELAY_BASED_WAH_WAH_H_

#include "components/mixer.h"
#include "components/low_frequency_oscillator.h"
#include "components/state_variable_filter.h"

typedef struct {
	LOWFREQOSC lfo;
	SVFILTER svf;
	MIXER mixer;
	float out;
	float wahFreq;
	float minCutoffFreq;
	float maxCutoffFreq;
}WAHWAH;
void initialize_WAHWAH(WAHWAH * ww, float wahFreq, float minCutOffFreq, float maxCutoffFreq,
		float damp, float wet_dry, float sampleRate);
float apply_WAHWAH (WAHWAH * ww, float input);
void setMInMAXCutoffFrequency (WAHWAH *ww, float minCutoffFreq, float maxCutoffFreq);
void setWahFreq_Damping_WAHWAH(WAHWAH * ww, float wahFreq, float damp);

#endif /* EFFECTS_DELAY_BASED_WAH_WAH_H_ */
