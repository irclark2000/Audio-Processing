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
#include "feedback_comb_filter.h"

void initialize_FBCF (FBCF *filter, float *buf, uint32_t buf_size, float feedbackGain, float sampleRate) {
#if AUDIO_EFFECTS_TESTER
	if (buf == 0) {
		buf = malloc(buf_size * sizeof(float));
	}
#endif
	memset(buf, 0, buf_size * sizeof(float));
	filter->sampleRate = sampleRate;
	filter->sampleTime = 1.0/sampleRate;  // 1/sampleRate: avoids division in process loop
	filter->fcbfOut = 0;
	setFeedback_FCFB(filter, feedbackGain);
	filter->buf = buf;
	filter->buf_size = buf_size;
	filter->index = 0;
}
void setFeedback_FBCF(FBCF *filter, float feedback) {
	feedback = MIN_MAX (feedback, -0.98, 0.98);
	filter->g0 = feedback;
}

float applyFilter_FBCF(FBCF *filter, float input) {
	filter->fcbfOut = input + filter->g0 * filter->buf[filter->index];
	filter->buf[filter->index] = filter->fcbfOut;
	filter->index = (filter->index + 1) % filter->buf_size;
	return filter->fcbfOut;
}

void unInitialize_FBCF (FBCF *filter) {
#if AUDIO_EFFECTS_TESTER
	free(filter->buf);
#endif
}
