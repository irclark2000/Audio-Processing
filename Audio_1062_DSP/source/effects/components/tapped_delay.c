/*
 * tapped_delay.c
 *
 *  Created on: Dec 22, 2024
 *      Author: isaac
 */
/*
Copyright 2024 Isaac R. Clark, Jr.

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

#include "tapped_delay.h"
#include "memory_debug.h"
#if !AUDIO_EFFECTS_TESTER
#include <cr_section_macros.h>
#endif

static const uint8_t NUM_TAPS = 19;
static float moorer_gain[] = {1.0, 0.841, 0.540, 0.491, 0.397, 0.380, 0.346, 0.289, 0.272,
		0.193, 0.192, 0.217, 0.181, 0.180, 0.181, 0.176, 0.142, 0.167, 0.134};

static float moorer_delay_sec[] = { 0.0, 0.0043, 0.0215, 0.0225, 0.0268, 0.0270, 0.0298, 0.0458, 0.0485,
		0.0572, 0.0587, 0.0595, 0.0612, 0.0707, 0.0708, 0.0726, 0.0741, 0.0753, 0.0797};

static uint32_t moorer_delay_buf_sizes[] = { 0, 0.0043 * 48000, 0.0215 * 48000, 0.0225 * 48000,
		0.0268 * 48000, 0.0270 * 48000, 0.0298 * 48000, 0.0458  * 48000, 0.0485  * 48000,
		0.0572 * 48000, 0.0587 * 48000, 0.0595  * 48000, 0.0612, 0.0707  * 48000,
		0.0708 * 48000, 0.0726, 0.0741 * 48000, 0.0753 * 48000, 0.0797 * 48000};


#if !AUDIO_EFFECTS_TESTER
__NOINIT(RAM3) static float moorer_td_buf [3825]; // 0.0797 * 48000
#endif

void initialize_TAPPED_DELAY (TAPPED_DELAY *td, float * buf, uint32_t * delay_buf_sizes, float *gain, uint8_t tap_count, float sampleRate){
	td->buf = buf;
	td->tap_count = tap_count;
	td->use_free = 0;
	td->tapped_out = 0;
	td->max_delay = 0;
	td->wrt_ptr = 0;
	for (uint8_t i=0; i < tap_count; ++i) {
		td->delay_offsets[i] = delay_buf_sizes[i];
		if (delay_buf_sizes[i] > td->max_delay) {
			td->max_delay = delay_buf_sizes[i];
		}
		td->gain[i] = gain[i];
	}
	for(uint32_t i=0; i < td->max_delay; ++i) {
		buf[i] = 0.0f;
	}
}
void initialize_MOORER_TAPPED_DELAY (TAPPED_DELAY *td, float sampleRate){
#if !AUDIO_EFFECTS_TESTER
	initialize_TAPPED_DELAY (td, moorer_td_buf, moorer_delay_buf_sizes, moorer_gain, NUM_TAPS, sampleRate);
#endif
}

void gui_initialize_MOORER_TAPPED_DELAY (TAPPED_DELAY *td, float sampleRate) {
	gui_initialize_TAPPED_DELAY (td, moorer_delay_sec, moorer_gain, NUM_TAPS, sampleRate);
}
void gui_initialize_TAPPED_DELAY (TAPPED_DELAY *td, float * delay_buf_Sec, float *gain, uint8_t tap_count, float sampleRate) {
	uint32_t * delay_buf_sizes = (uint32_t *)MALLOC(sizeof(uint32_t)* tap_count);
	uint32_t max_delay  = 0;
	td->tap_count = tap_count;
	for (uint8_t i=0; i < tap_count; ++i) {
		delay_buf_sizes[i] = delay_buf_Sec[i] * sampleRate;
		if (delay_buf_sizes[i] > max_delay) {
			max_delay = delay_buf_sizes[i];
		}
	}
	float *buf = (float *)MALLOC(sizeof(float)* max_delay);
	initialize_TAPPED_DELAY (td, buf, delay_buf_sizes, gain, tap_count, sampleRate);
	FREE(delay_buf_sizes);
	td->use_free = 1;
}
float update_TAPPED_DELAY(TAPPED_DELAY *td, float input) {
	td->accumulator = 0.0f;
	for (uint8_t i=0; i < td->tap_count; ++i) {
		if (td->delay_offsets[i] == td->max_delay) {
			td->accumulator += td->buf[td->wrt_ptr] * td->gain[i];
		}
		else if (td->delay_offsets[i] == 0) {
			td->accumulator += input * td->gain[i];
		}
		else {
			uint32_t offset = (td->wrt_ptr + 2 * td->max_delay - td->delay_offsets[i]) % td->max_delay;
			td->accumulator += td->buf[offset] * td->gain[i];
		}
	}
	td->tapped_out = td->accumulator;
	td->buf[td->wrt_ptr] = input;
	td->wrt_ptr = (td->wrt_ptr + 1) % td->max_delay;
	return td->tapped_out;
}
void uninitialize_TAPPED_DELAY(TAPPED_DELAY *td) {
	if (td->use_free != 0) {
		FREE(td->buf);
	}
}
