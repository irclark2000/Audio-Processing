/*
 * envelope_follower.h
 *
 *  Created on: Nov 1, 2024
 *      Author: isaac
 */
/*
 Copyright 2022 Isaac R. Clark, Jr.

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
 files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy,
 modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef EFFECTS_COMPONENTS_ENVELOPE_FOLLOWER_H_
#define EFFECTS_COMPONENTS_ENVELOPE_FOLLOWER_H_
//#include "leaky_integrator.h"
#include "fast_math/fast_math.h"

typedef struct {
	float efOut;
	float sampleRate;
	float attack_parameter;
	float release_parameter;
	float absInput;
	float gui_attack_time;
	float gui_release_time;
}ENVELOPE_FOLLOWER;

void initialize_ENVELOPE_FOLLOWER(ENVELOPE_FOLLOWER *ef, float sampleRate);
float apply_ENVELOPE_FOLLOWER(ENVELOPE_FOLLOWER *ef, float input);
void gui_setAttackRelease_ENVELOPE_FOLLOWER(ENVELOPE_FOLLOWER *ef);
#endif /* EFFECTS_COMPONENTS_ENVELOPE_FOLLOWER_H_ */
