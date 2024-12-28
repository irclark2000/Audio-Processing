/*
 * moorer_reverb.h
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

#ifndef EFFECTS_REVERBS_MOORER_REVERB_H_
#define EFFECTS_REVERBS_MOORER_REVERB_H_

#if !AUDIO_EFFECTS_TESTER
#include <cr_section_macros.h>
#endif

#define COMB_DELAY0 50
#define COMB_DELAY1 56
#define COMB_DELAY2 61
#define COMB_DELAY3 68
#define COMB_DELAY4 72
#define COMB_DELAY5 78

#define COMB_BUF_SIZE_0 50 * 48
#define COMB_BUF_SIZE_1 56 * 48
#define COMB_BUF_SIZE_2 61 * 48
#define COMB_BUF_SIZE_3 68 * 48
#define COMB_BUF_SIZE_4 72 * 48
#define COMB_BUF_SIZE_5 78 * 48

#define NUMBER_COMB_FILTERS 6
#define NUMBER_ALL_PASS_FILTERS 2
#include <filters/all_pass1.h>
#include "filters/moorer_comb.h"
#include "effects/components/mixer.h"
#include "effects/components/tapped_delay.h"

typedef struct MOORER_REVERB {
	TAPPED_DELAY td;
	MOORER_COMB comb_filter[NUMBER_COMB_FILTERS];
	ALLPASS1 allpass;
	uint8_t use_free;
	float out;
	float comb_out;

} MOORER_REVERB;

void intialize_MOORER_REVERB(MOORER_REVERB *mr, float **comb_buf, uint32_t * sizes, float * allpassbuf, uint32_t allpass_size, float sampleRate);
void gui_initialize_MOORER_REVERB(MOORER_REVERB *mr, float sampleRate);
float apply_MOORER_REVERB(MOORER_REVERB *mr, float input);
void uninitialize_MOORER_REVERB(MOORER_REVERB *mr);
#endif /* EFFECTS_REVERBS_MOORER_REVERB_H_ */
