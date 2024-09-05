/*
 * noise_gate.c
 *
 *  Created on: May 21, 2022
 *      Author: isaac
 *      Based on code written by Philip Salmony
 *      See YouTube Phil's Lab Episode #58
 */


#include "noise_gate.h"

// Copyright 2021 Johan Rade (johan.rade@gmail.com)
// Distributed under the MIT license (https://opensource.org/licenses/MIT)
#define EXP(x) fastExp(x)

float fastExp(float x)
{
    float a = (1 << 23) / 0.69314718f;
    float b = (1 << 23) * (127 - 0.043677448f);
    x = a * x + b;

#if 0
    // Remove these lines if bounds checking is not needed
    constexpr float c = (1 << 23);
    constexpr float d = (1 << 23) * 255;
    if (x < c || x > d)
        x = (x < c) ? 0.0f : d;
#endif
    // With C++20 one can use std::bit_cast instead
    uint32_t n = (uint32_t)(x);
    memcpy(&x, &n, 4);
    return x;
}


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

