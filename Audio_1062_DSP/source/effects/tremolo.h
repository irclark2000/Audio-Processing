/*
 * tremolo.h
 *
 *  Created on: Apr 3, 2022
 *      Author: isaac
 *      Based on code written by Philip Salmony
 *      See YouTube Phil's Lab Episode #51
 */

#ifndef TREMOLO_H_
#define TREMOLO_H_
#include "components/low_frequency_oscillator.h"
#include "components/mixer.h"
#if defined(__cplusplus)
extern "C" {
#endif


typedef struct {
	float depth;      // 0 to 1.0
	LOWFREQOSC osc;
	float sampleRate;  //Hz
	float tOut;        // avoids declaring variable inside a function
	MIXER mixer;
} TREMOLO;

void initialize_TREMOLO(TREMOLO * trem, float depth, float tremFreq, float sampleRate);
void setDepth_TREMOLO(TREMOLO * trem, float depth);
void setTremFrequency_TREMOLO(TREMOLO * trem, float tremFreq);
float update_TREMOLO(TREMOLO *trem, float input, int increment);
float gui_update_TREMOLO(TREMOLO *trem, float input);
#if defined(__cplusplus)
}
#endif

#endif /* TREMOLO_H_ */
