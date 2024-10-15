/*
 * noise_gate.h
 *
 *  Created on: May 21, 2022
 *      Author: isaac
 *      Based on code written by Philip Salmony
 *      See YouTube Phil's Lab Episode #58
 */

#ifndef NOISE_GATE_H_
#define NOISE_GATE_H_

typedef struct {
	float threshold;
	float absInput;
	float out;
	float holdTimeMs;
	float sampleTimeMs;
	float attack_alpha;   // gain reducing towards 0
	float release_alpha;  // gain increasing towards 1
	float attack_time_accumulator;
	float release_time_accumulator;
	float smooth_gain;
	float static_gain;

} NOISEGATE;

void initializeNoiseGate(NOISEGATE *ng, float attackTimeMs, float releaseTimeMs, float holdTimeMs, float sampleRate, float threshold);
float applyNoiseGate(NOISEGATE *ng, float input);
void setNoiseAttackRelease(NOISEGATE *ng, float attackMs, float releaseMs);
void setNoiseThreshold(NOISEGATE *ng, float threshold);
#endif /* NOISE_GATE_H_ */
