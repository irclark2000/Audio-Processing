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

#include <dynamic_range_control/limiter.h>
#include <fast_math/fast_math.h>
#include "components/effects_macros.h"
#define LIMITER_MINTHRESHOLD_DB -50
#define LIMITER_MAXTHRESHOLD_DB 0
#define LIMITER_MINRATIO 1
#define LIMITER_MAXRATIO 50
#define LIMITER_MINKNEEWIDTH 0
#define LIMITER_MAXKNEEWIDTH 20
#define LIMITER_MINRELEASE_SEC 0
#define LIMITER_MAXRELEASE_SEC 4
#define LIMITER_MINATTACK_SEC 0
#define LIMITER_MAXATTACK_SEC 4
#define LIMITER_MINMAKEUPGAIN_DB -10
#define LIMITER_MAXMAKEUPGAIN_DB 24


//static const float log10_9 = 0.95424250943f;
static const float ln9 = 2.19722457734f;
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
	limiter->threshold_db = -10.0f;  //db
	limiter->knee = 0.0f;
	limiter->makeup_gain = 0.0f;
	limiter->makeup_property_mode = 1;
	limiter->compress_out = 0.0f;
	setRelease_LIMITER(limiter, 0.2f);
	setAttack_LIMITER(limiter, 0.0f);
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
	if (limiter->knee < 0.01) {
		if (xdb < limiter->threshold_db) {
			xsc = xdb;
		} else {
			xsc = limiter->threshold_db;
		}
	} else {
		if (xdb < (limiter->threshold_db - limiter->knee / 2.0f)) {
			xsc = xdb;
		} else if (xdb > (limiter->threshold_db + limiter->knee / 2.0f)) {
			xsc = limiter->threshold_db;
		} else {
			xsc = xdb
					- (xdb - limiter->threshold_db + limiter->knee / 2.0f)
					* (xdb - limiter->threshold_db + limiter->knee / 2.0f) / 2.0f
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
void gui_setAttackRelease_LIMITER(LIMITER *limit) {
	setRelease_LIMITER(limit,limit->release_time);
	setAttack_LIMITER(limit, limit->attack_time);
}

void setRelease_LIMITER(LIMITER *limit, float release_time) {
	release_time = MIN_MAX(release_time, LIMITER_MINRELEASE_SEC, LIMITER_MAXRELEASE_SEC);
	limit->release_time = release_time;
	limit->alphaR = EXP(-ln9 * limit->sample_time / release_time);
}
void setAttack_LIMITER(LIMITER *limit, float attack_time) {
	attack_time = MIN_MAX(attack_time, LIMITER_MINATTACK_SEC, LIMITER_MAXATTACK_SEC);
	limit->attack_time = attack_time;
	limit->alphaA = EXP(-ln9 * limit->sample_time / attack_time);
}
void setTreshold_LIMITER(LIMITER * limit, float threshold_db) {
	threshold_db = MIN_MAX(threshold_db, LIMITER_MINTHRESHOLD_DB, LIMITER_MAXTHRESHOLD_DB);
	limit->threshold_db = threshold_db;
}
void setKneeWidth_LIMITER(LIMITER * limit, float knee_width) {
	knee_width = MIN_MAX(knee_width, LIMITER_MINKNEEWIDTH, LIMITER_MAXKNEEWIDTH);
	limit->knee = knee_width;
}
void setMakeupGainDB_LIMITER(LIMITER * limit, float makeup_gain) {
	makeup_gain = MIN_MAX(makeup_gain, LIMITER_MINMAKEUPGAIN_DB , LIMITER_MAXMAKEUPGAIN_DB);
	limit->makeup_gain = makeup_gain;
}
void gui_setMakeupPropertyKnee_LIMITER (LIMITER *limiter) {
	limiter->makeup_property_mode = (limiter->gui_not_hard_property == 0) ? 1 : 0;
}

