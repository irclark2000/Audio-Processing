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

#if defined(__cplusplus)
extern "C" {
#endif


typedef struct {
	float depth;      // 0 to 1.0
	float tCount;
	float tCountLimit;
	float tDir;     // -1 down, 1 up

	float sampleRate;  //Hz
	float tOut;        // avoid declaring memory in a function
} TREMOLO;

void Tremolo_Init(TREMOLO * trem, float depth, float tremFreq, float sampleRate);
void Tremolo_SetDepth(TREMOLO * trem, float depth);
void Tremolo_SetTremFrequency(TREMOLO * trem, float tremFreq);
float Tremolo_Update(TREMOLO *trem, float input, int increment);
#if defined(__cplusplus)
}
#endif

#endif /* TREMOLO_H_ */