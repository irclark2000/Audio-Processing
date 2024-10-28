/*
 * feedback_comb_filter.c
 *
 *  Created on: Oct 24, 2024
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
#include "feedforward_comb_filter.h"

void initialize_FFCF (FFCF *filter, float *buf, uint32_t buf_size, float feedforwardGain, float directLevel, float sampleRate) {
#if AUDIO_EFFECTS_TESTER
	if (buf == 0) {
		buf = malloc(buf_size * sizeof(float));
	}
#endif
	memset(buf, 0, buf_size * sizeof(float));
	filter->sampleRate = sampleRate;
	filter->sampleTime = 1.0/sampleRate;  // 1/sampleRate: avoids division in process loop
	filter->fcbfOut = 0;
	setFeedforward_FFCF(filter, feedforwardGain);
	setDirectLevel_FFCF(filter, directLevel);
	filter->buf = buf;
	filter->buf_size = buf_size;
	filter->index = 0;
}
void setFeedforward_FFCF(FFCF *filter, float feedforward) {
	feedforward = MIN_MAX (feedforward, -0.98, 0.98);
	filter->bM = feedforward;
}
void setDirectLevel_FFCF(FFCF *filter, float direct) {
	direct = MIN_MAX (direct, 0, 1.0);
	filter->b0 = direct;
}
float applyFilter_FFCF(FFCF *filter, float input) {
	filter->fcbfOut = input * filter->b0 + filter->bM * filter->buf[filter->index];
	filter->buf[filter->index] = input;
	filter->index = (filter->index + 1) % filter->buf_size;
	return filter->fcbfOut;
}

void unInitialize_FFCF (FFCF *filter) {
#if AUDIO_EFFECTS_TESTER
	free(filter->buf);
#endif
}
/*
 * feedforward_comb_filter.c
 *
 *  Created on: Oct 27, 2024
 *      Author: isaac
 */


