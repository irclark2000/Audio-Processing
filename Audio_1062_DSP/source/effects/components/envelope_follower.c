/*
 * envelope_follower.c
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

#include "envelope_follower.h"

void initialize_ENVELOPE_FOLLOWER(ENVELOPE_FOLLOWER *ef, float sampleRate) {
	ef->sampleRate = sampleRate;
	ef->efOut = 0.0f;
	ef->attack_parameter = 0.5;
	ef->release_parameter = 0.999;
}
float apply_ENVELOPE_FOLLOWER(ENVELOPE_FOLLOWER *ef, float input) {
	ef->absInput = input;
	if (input < 0.0f) {
		ef->absInput = -input;
	}
	if (ef->absInput > ef->efOut) {
		ef->efOut = ef->attack_parameter * ef->efOut + (1.0f - ef->attack_parameter) * ef->absInput;
	}
	else {
		ef->efOut = ef->release_parameter * ef->efOut + (1.0f - ef->release_parameter) * ef->absInput;
	}
	return ef->efOut;
}
void gui_setAttackRelease_ENVELOPE_FOLLOWER(ENVELOPE_FOLLOWER *ef) {
	ef->attack_parameter = fastExp(-1.0f / (ef->gui_attack_time * ef->sampleRate));
    ef->release_parameter = fastExp(-1.0f / (ef->gui_release_time * ef->sampleRate));
}

