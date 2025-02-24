/*
 * effects_chain.c
 *
 *  Created on: Feb 3, 2025
 *      Author: isaac
 */

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
#include "effects_chain.h"
#include <stdint.h>

void initialize_EFFECTS_CHAIN (EFFECTS_CHAIN * chain) {
	chain->length = 0;
	chain->min_channels = -1;
}


void clear_EFFECTS_CHAIN (EFFECTS_CHAIN * chain) {
	for (uint8_t i=0; i < chain->length; ++i) {
		uninitialize_AUDIO_COMPONENT(chain->audio_components[i]);
	}
	chain->length = 0;
	chain->min_channels = -1;
}
void addComponent_EFFECTS_CHAIN (EFFECTS_CHAIN * chain, AUDIO_COMPONENT * ac) {
	if (chain->length < MAX_CHAIN_LENGTH) {
		chain->audio_components[chain->length] = ac;
		chain->length++;
	}
	if (chain->min_channels == -1) {
		chain->min_channels = ac->channel_count;
	}
	else if (ac->channel_count < chain->min_channels){
		chain->min_channels = ac->channel_count;
	}
}
float play_audio_EFFECTS_CHAIN (EFFECTS_CHAIN * chain, float input, float prevChanInput, float prevChanOutput, uint8_t channel) {
	if (chain->min_channels <= channel) {
		return prevChanOutput;
	}
	float output = input;
	for (uint8_t chain_number = 0; chain_number < chain->length; ++chain_number) {
		AUDIO_COMPONENT * ac = chain->audio_components[chain_number];
		EFFECT_COMPONENT *ec = ac->channel[channel];
		output = ec->apply(ec, output);
	}
	return output;
}

