/*
 * tremolo.c
 *
 *  Created on: Apr 3, 2022
 *      Author: isaac
 *      Based on code written by Philip Salmony
 *      See YouTube Phil's Lab Episode #51
 **/

#include "tremolo.h"

void Tremolo_Init(TREMOLO * trem, float depth, float tremFreq, float sampleRate) {
	trem->sampleRate = sampleRate;
	Tremolo_SetDepth (trem, depth);
	Tremolo_SetTremFrequency (trem, tremFreq);
	trem->tCount = 0.0;
	trem->tDir = 1.0;
}
void Tremolo_SetDepth(TREMOLO * trem, float depth) {
	if (depth > 1.0f) {
		depth = 1.0f;
	}
	else if (depth < 0.0f) {
		depth = 0.0f;
	}
	trem->depth = depth;
}
void Tremolo_SetTremFrequency(TREMOLO * trem, float tremFreq) {
	if (tremFreq <= 0.0f) {
		tremFreq = 1.0f;
	}
	else if (tremFreq > 0.5f * trem->sampleRate) {
		tremFreq = 0.5f * trem->sampleRate;
	}
	trem->tCountLimit = 0.25f * (trem->sampleRate / tremFreq);

	// may have to adjust the count to keep it in bounds
	if (trem->tCount > trem->tCountLimit) {
		trem->tCount = trem->tCountLimit;
	}
	else if (trem->tCount < -trem->tCountLimit)  {
		trem->tCount = -trem->tCountLimit;
	}
}

float Tremolo_Update(TREMOLO *trem, float input, int increment) {
	trem->tOut = input * ((1.0f - trem->depth) + trem->depth *  (trem->tCount / trem->tCountLimit));

	// update the direction if needed
	if (trem->tCount >= trem->tCountLimit) {
		trem->tDir = -1.0f;
	}
	if (trem->tCount <= -trem->tCountLimit) {
		trem->tDir = 1.0f;
	}
	// bump the count
	if (increment != 0) {
		trem->tCount += trem->tDir;
	}
	return trem->tOut;
}