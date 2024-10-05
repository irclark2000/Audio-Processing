/*
 * tremolo.c
 *
 *  Created on: Apr 3, 2022
 *      Author: isaac
 *      Based on code written by Philip Salmony
 *      See YouTube Phil's Lab Episode #51
 **/

#include "tremolo.h"

void initialize_TREMOLO(TREMOLO * trem, float depth, float tremFreq, float sampleRate) {
	setDepth_TREMOLO (trem, depth);
	initialize_LOWFREQOSC(&(trem->osc), 1.0f, 0.0f, 30, tremFreq, 0.01f, 4000.0f, 0.0f, sampleRate);
}

void setDepth_TREMOLO(TREMOLO * trem, float depth) {
	if (depth > 1.0f) {
		depth = 1.0f;
	}
	else if (depth < 0.0f) {
		depth = 0.0f;
	}
	trem->depth = depth;
}
void setTremFrequency_TREMOLO(TREMOLO * trem, float tremFreq) {
	setFreq_LOWFREQOSC(&(trem->osc), tremFreq);
}

float update_TREMOLO(TREMOLO *trem, float input, int increment) {
	update_LOWFREQOSC(&(trem->osc));
	trem->tOut = input * ((1.0f - trem->depth) + trem->depth * trem->osc.out);
	return trem->tOut;
}
