/*
 * moorer_reverb.c
 *
 *  Created on: Dec 19, 2024
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


#include "moorer_reverb.h"

#if !AUDIO_EFFECTS_TESTER
__NOINIT(RAM3) static float comb0_buf[COMB_BUF_SIZE_0];
__NOINIT(RAM3) static float comb1_buf[COMB_BUF_SIZE_1];
__NOINIT(RAM3) static float comb2_buf[COMB_BUF_SIZE_2];
__NOINIT(RAM3) static float comb3_buf[COMB_BUF_SIZE_3];
__NOINIT(RAM3) static float comb4_buf[COMB_BUF_SIZE_4];
__NOINIT(RAM3) static float comb5_buf[COMB_BUF_SIZE_5];

__NOINIT(RAM3) static float ap0_buf[60 * 48];
#endif

// gain values from About This Reverberation Business, James Moorer, 1979
static float gain_25k[] = {0.24f, 0.26f, 0.28f, 0.29f, 0.30f, 0.32f};
static float gain_50k[] = {0.46f, 0.48f, 0.50f, 0.52f, 0.53f, 0.55f};
static float low_pass_frequency[] = {1942.0f, 1363.0f, 1312.0f, 1574.0f, 981.0f, 1036.0f};
static uint32_t buf_sizes[] = {COMB_BUF_SIZE_0, COMB_BUF_SIZE_1, COMB_BUF_SIZE_2, COMB_BUF_SIZE_3, COMB_BUF_SIZE_4, COMB_BUF_SIZE_5};

float calculate_gain_by_interpolation (uint8_t index, float sampleRate) {
	return gain_25k[index] + (gain_50k[index] - gain_25k[index]) * (sampleRate - 25000.0f)/ 25000.0f;
}

void intialize_MOORER_REVERB(MOORER_REVERB *mr, float **buf, uint32_t * sizes, float * allpassbuf, uint32_t allpass_size, float sampleRate) {

#if AUDIO_EFFECTS_TESTER
	gui_initialize_MOORER_TAPPED_DELAY (&(mr->td), sampleRate);
	for(uint8_t i=0; i < NUMBER_COMB_FILTERS; ++i) {
		initialize_MOORER_COMB(&(mr->mr->comb_filter[i]), 0, buf_sizes[i], low_pass_frequency[i], calculate_gain_by_interpolation (0, sampleRate), sampleRate);
	}
	initAllpassFilter1(&(mr->allpass), 0, 60 * 48, 0.7f);
	mr->use_free = 1;
#else
	initialize_MOORER_TAPPED_DELAY (&(mr->td), sampleRate);
	initialize_MOORER_COMB(&(mr->comb_filter[0]), comb0_buf, COMB_BUF_SIZE_0, low_pass_frequency[0], calculate_gain_by_interpolation (0, sampleRate), sampleRate);
	initialize_MOORER_COMB(&(mr->comb_filter[1]), comb1_buf, COMB_BUF_SIZE_1, low_pass_frequency[1], calculate_gain_by_interpolation (1, sampleRate), sampleRate);
	initialize_MOORER_COMB(&(mr->comb_filter[2]), comb2_buf, COMB_BUF_SIZE_2, low_pass_frequency[2], calculate_gain_by_interpolation (2, sampleRate), sampleRate);
	initialize_MOORER_COMB(&(mr->comb_filter[3]), comb3_buf, COMB_BUF_SIZE_3, low_pass_frequency[3], calculate_gain_by_interpolation (3, sampleRate), sampleRate);
	initialize_MOORER_COMB(&(mr->comb_filter[4]), comb4_buf, COMB_BUF_SIZE_4, low_pass_frequency[4], calculate_gain_by_interpolation (4, sampleRate), sampleRate);
	initialize_MOORER_COMB(&(mr->comb_filter[5]), comb5_buf, COMB_BUF_SIZE_5, low_pass_frequency[5], calculate_gain_by_interpolation (5, sampleRate), sampleRate);
	initAllpassFilter1(&(mr->allpass), ap0_buf, 60 * 48, 0.7f);
	mr->use_free = 0;
#endif
	initialize_MIXER (&(mr->mixer), sampleRate);
}

void gui_initialize_MOORER_REVERB(MOORER_REVERB *mr, float sampleRate) {
	intialize_MOORER_REVERB(mr, 0, buf_sizes, 0, 19, sampleRate);

}

float apply_MOORER_REVERB(MOORER_REVERB *mr, float input) {
	mr->out = update_TAPPED_DELAY(&(mr->td), input);
	mr->comb_out = 0;
	for(uint8_t i=0; i < NUMBER_COMB_FILTERS; ++i) {
		mr->comb_out += apply_MOORER_COMB(&(mr->comb_filter[i]), mr->out);
	}
	mr->out = applyAllpassFilter1(&(mr->allpass), mr->comb_out);
	mr->out = applyWetDry_MIXER (&(mr->mixer), mr->out,  input);

	return mr->out;
}
void uninitialize_MOORER_REVERB(MOORER_REVERB *mr) {
	if (mr->use_free) {
		uninitialize_TAPPED_DELAY(&(mr->td));
		for(uint8_t i=0; i < NUMBER_COMB_FILTERS; ++i) {
			uninitialize_MOORER_COMB(&(mr->comb_filter[i]));
		}
		uninitialize_AllpassFilter1(&(mr->allpass));
	}
}
