/*
 * effects_chain.h
 *
 *  Created on: Feb 3, 2025
 *      Author: isaac
 */

#ifndef EFFECTS_TESTER_AUDIO_PLAYER_EFFECTS_CHAIN_H_
#define EFFECTS_TESTER_AUDIO_PLAYER_EFFECTS_CHAIN_H_

/*
 Copyright 2024 Isaac R. Clark, Jr.

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
 fiiles (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy,
 modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "audio_component.h"
#include <stdint.h>


#define MAX_CHAIN_LENGTH 15

typedef struct EFFECTS_CHAIN {
	AUDIO_COMPONENT * audio_components[MAX_CHAIN_LENGTH];
	uint8_t length;
	int8_t min_channels;
} EFFECTS_CHAIN;

void initialize_EFFECTS_CHAIN (EFFECTS_CHAIN * chain);
void clear_EFFECTS_CHAIN (EFFECTS_CHAIN * chain);
void addComponent_EFFECTS_CHAIN (EFFECTS_CHAIN * chain, AUDIO_COMPONENT * ac);
float play_audio_EFFECTS_CHAIN (EFFECTS_CHAIN * chain, float input, float prevChanInput, float prevChanOutput, uint8_t channel);

#endif /* EFFECTS_TESTER_AUDIO_PLAYER_EFFECTS_CHAIN_H_ */
