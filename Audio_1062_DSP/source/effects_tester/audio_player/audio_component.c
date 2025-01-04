/*
 * audio_component.c
 *
 *  Created on: Dec 31, 2024
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
#include "audio_component.h"

void initialize_AUDIO_COMPONENT(AUDIO_COMPONENT *ac) {
	ac->channel_count = 0;
}
void add_component(AUDIO_COMPONENT *ac, EFFECT_COMPONENT *ec) {
	if (MAX_NUMBER_CHANNELS > ac->channel_count) {
		ac->channel[ac->channel_count++] = ec;
	}
}
void uninitialize_AUDIO_COMPONENT(AUDIO_COMPONENT *ac) {
	clear_AUDIO_COMPONENT(ac);
	free(ac);
}
void clear_AUDIO_COMPONENT(AUDIO_COMPONENT *ac) {
	for (uint8_t i = 0; i < ac->channel_count; ++i) {
		freeComponent(ac->channel[i]);
	}
	ac->channel_count = 0;
}
