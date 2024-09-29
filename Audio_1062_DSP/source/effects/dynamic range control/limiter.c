/*
 * limiter.c
 *
 *  Created on: Sep 19, 2024
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

#include <dynamic range control/limiter.h>
#include "fast_math.h"

static float log10_9 = 0.95424250943f;
static float ln9 = 2.19722457734f;
#define TRY_FAST 0  // using fast approximations yields substantially better performance
#if TRY_FAST
#define EXP fastExp
#define LOG10 fastLog10
#else
#define EXP expf
#define LOG10 log10f
#endif



float limiter_gain_calc_smoothing(LIMITER *limiter, float xdb, float *xscOut, float *gcOut);

void initialize_LIMITER(LIMITER *limiter, float sample_rate) {
	limiter->sample_time = 1.0f / sample_rate;
	limiter->gs = 0.0f;
	limiter->threshold = -10.0f;  //db
	limiter->knee = 0.0f;
	limiter->makeup_gain = 0.0f;
	limiter->makeup_property_mode = 1;
	limiter->compress_out = 0.0f;
	limiter->hard_knee = 0;
	limiter_setRelease(limiter, 0.2f);
	limiter_setAttack(limiter, 0.0f);
}
// based on MATLAB limiter

float update_LIMITER(LIMITER *limiter, float input) {
	limiter->absInput = input;
	if (input < 0.0f) {
		limiter->absInput = -limiter->absInput;
	}
	float xdb = 20.0f * LOG10(limiter->absInput);

	if (isINF(xdb)) {  // result of log(0)
		return input;
	}
	float xsc;
	float gc;
	float gs = limiter_gain_calc_smoothing(limiter, xdb, &xsc, &gc);
	if (gs != gs) {
		return input;
	}
	// make-up gain
	float M = limiter->makeup_gain;
	if (!limiter->makeup_property_mode) { // auto mode
		float xsc0;
		// calculate using xdb = 0
		limiter_gain_calc_smoothing(limiter, 0.0f, &xsc0, NULL);
		M = -xsc0;
	}
	limiter->gs = gs;
	float gm = gs + M;
	// linear gain
#if TRY_FAST
	float glin = fastPow10(gm / 20.0f);
#else
	float glin = powf(10.0, gm / 20.0f);
#endif
	limiter->compress_out = glin * input;
	return limiter->compress_out;
}

float limiter_gain_calc_smoothing(LIMITER *limiter, float xdb, float *xscOut, float *gcOut) {
	float xsc;
	if (limiter->hard_knee || limiter->knee < 0.05) {
		if (xdb < limiter->threshold) {
			xsc = xdb;
		} else {
			xsc = limiter->threshold;
		}
	} else {
		if (xdb < (limiter->threshold - limiter->knee / 2.0f)) {
			xsc = xdb;
		} else if (xdb > (limiter->threshold + limiter->knee / 2.0f)) {
			xsc = limiter->threshold;
		} else {
			xsc = xdb
					- (xdb - limiter->threshold + limiter->knee / 2.0f)
					* (xdb - limiter->threshold + limiter->knee / 2.0f) / 2.0f
							/ limiter->knee;
		}
	}
	float gc = xsc - xdb;
	float gs;
	if (xscOut) *xscOut = xsc;
	if (gcOut) *gcOut = gc;
	// gain smoothing
	if (gc <= limiter->gs) {
		gs = limiter->alphaA * limiter->gs + (1.0f - limiter->alphaA) * gc;
	} else {
		gs = limiter->alphaR * limiter->gs + (1.0 - limiter->alphaR) * gc;
	}
	return gs;
}

void limiter_setRelease(LIMITER *limiter, float release_time) {
	limiter->release_time = release_time;
	limiter->alphaR = EXP(-ln9 * limiter->sample_time / release_time);
}
void limiter_setAttack(LIMITER *limiter, float attack_time) {
	limiter->attack_time = attack_time;
	limiter->alphaA = EXP(-ln9 * limiter->sample_time / attack_time);
}
