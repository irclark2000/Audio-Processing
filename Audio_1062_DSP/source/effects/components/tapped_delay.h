/*
 * tapped_delay.h
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
#ifndef EFFECTS_COMPONENTS_TAPPED_DELAY_H_
#define EFFECTS_COMPONENTS_TAPPED_DELAY_H_
#include <stdint.h>

#define MAXIMUM_TAP_COUNT 100
typedef struct TAPPED_DELAY {
	uint32_t delay_offsets[MAXIMUM_TAP_COUNT];
	float gain[MAXIMUM_TAP_COUNT];
	uint32_t max_delay;
	float tapped_out;
	float accumulator;
	uint8_t tap_count;
	float * buf;
	int32_t wrt_ptr;
	uint8_t use_free;
} TAPPED_DELAY;

void initialize_TAPPED_DELAY (TAPPED_DELAY *td, float * buf, uint32_t * delay_buf_sizes, float *gain, uint8_t tap_count, float sampleRate);
void gui_initialize_TAPPED_DELAY (TAPPED_DELAY *td, float * delay_buf_mSec, float *gain, uint8_t tap_count, float sampleRate);
void gui_initialize_MOORER_TAPPED_DELAY (TAPPED_DELAY *td, float sampleRate);
float update_TAPPED_DELAY(TAPPED_DELAY *td, float input);
void initialize_MOORER_TAPPED_DELAY (TAPPED_DELAY *td, float sampleRate);

void uninitialize_TAPPED_DELAY(TAPPED_DELAY *td);


#endif /* EFFECTS_COMPONENTS_TAPPED_DELAY_H_ */
