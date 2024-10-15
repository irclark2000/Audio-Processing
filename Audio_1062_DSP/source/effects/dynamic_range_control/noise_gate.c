/*
 * noise_gate.c
 *
 *  Created on: May 21, 2022
 *      Author: isaac
 *      Based on code written by Philip Salmony
 *      See YouTube Phil's Lab Episode #58
 */


#include <dynamic_range_control/noise_gate.h>
#include <fast_math/fast_math.h>

#define EXP(x) fastExp(x)


void initializeNoiseGate(NOISEGATE *ng, float attackTimeMs, float releaseTimeMs, float holdTimeMs, float sampleRate, float threshold) {
	ng->smooth_gain = 0.0f;
	ng->static_gain = 1.0f;
	ng->holdTimeMs = holdTimeMs;
	ng->sampleTimeMs = 1000.0f/sampleRate;
	setNoiseThreshold(ng, threshold);
	ng->attack_time_accumulator = 0.0f;
	ng->release_time_accumulator = 0.0f;
	setNoiseAttackRelease(ng, attackTimeMs, releaseTimeMs);
}

float applyNoiseGate(NOISEGATE *ng, float input) {
	ng->absInput = input;
	if (input < 0)  {
		ng->absInput = -input;
	}
	ng->static_gain = 0.0f;
	if (ng->absInput >= ng->threshold) {
		ng->static_gain = 1.0f;
	}
	//replace the next line with the filtering code
	//ng->smooth_gain = ng->static_gain;
	if (ng->static_gain < ng->smooth_gain) { // reducing to zero (attack)
		if (ng->attack_time_accumulator > ng->holdTimeMs) {
			ng->smooth_gain = ng->attack_alpha * ng->smooth_gain + (1.0f - ng->attack_alpha) * ng->static_gain;
		}
		// no reducing until after hold time
		else {
			ng->attack_time_accumulator += ng->sampleTimeMs;
		}
	}
	else { // time to raise gain
		ng->attack_time_accumulator = 0;
		ng->smooth_gain = ng->release_alpha * ng->smooth_gain + (1.0f - ng->release_alpha) * ng->static_gain;
	}


	ng->out = ng->smooth_gain * input;
	return ng->out;
}

void setNoiseThreshold(NOISEGATE *ng, float threshold) {
	if (threshold < 0) {
		threshold = 0;
	}
	else if (threshold > 1.0f) {
		threshold = 1.0f;
	}
	ng->threshold = threshold;

}
// exponential function requires fast math
void setNoiseAttackRelease(NOISEGATE *ng, float attackMs, float releaseMs) {
	ng->attack_alpha =  EXP(-2.2f/attackMs/ng->sampleTimeMs);
	ng->release_alpha = EXP(-2.2f/releaseMs/ng->sampleTimeMs);
}

