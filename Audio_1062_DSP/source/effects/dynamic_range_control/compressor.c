/*
 * compressor.c
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

#include <dynamic_range_control/compressor.h>
//static const float log10_9 = 0.95424250943f;
static const float ln9 = 2.19722457734f;

#include <fast_math/fast_math.h>
#include "components/effects_macros.h"
#define COMPRESSOR_MINTHRESHOLD_DB -50
#define COMPRESSOR_MAXTHRESHOLD_DB 0
#define COMPRESSOR_MINRATIO 1
#define COMPRESSOR_MAXRATIO 50
#define COMPRESSOR_MINKNEEWIDTH 0
#define COMPRESSOR_MAXKNEEWIDTH 20
#define COMPRESSOR_MINRELEASE_SEC 0
#define COMPRESSOR_MAXRELEASE_SEC 4
#define COMPRESSOR_MINATTACK_SEC 0
#define COMPRESSOR_MAXATTACK_SEC 4
#define COMPRESSOR_MINMAKEUPGAIN_DB -10
#define COMPRESSOR_MAXMAKEUPGAIN_DB 24

#define TRY_FAST 0
#if TRY_FAST
#define EXP fastExp
#define LOG10 fastLog10
#else
#define EXP expf
#define LOG10 log10f
#endif


float compressor_gain_calc_smoothing(COMPRESSOR *comp, float xdb, float *xscOut, float * gcOut);
void initialize_COMPRESSOR(COMPRESSOR *comp, float sample_rate) {
	comp->sample_time = 1.0f / sample_rate;
	comp->gs = 0.0f;
	comp->inv_ratio = 1.0f / 5.0f;
	comp->threshold = -10.0f;  //db
	comp->knee = 0.0f;
	comp->makeup_gain = 0.0f;
	comp->makeup_property_mode = 1;
	comp->compress_out = 0.0f;
	comp->hard_knee = 0;
	setRelease_COMPRESSOR(comp, 0.2f);
	setAttack_COMPRESSOR(comp, 0.05f);
}
// based on MATLAB compressor

float update_COMPRESSOR (COMPRESSOR *comp, float input) {
	comp->absInput = input;
	if (input < 0.0f) {
		comp->absInput = -comp->absInput;
	}
	float xdb = 20.0f * LOG10(comp->absInput);
	if (isINF(xdb)) {
		return input;
	}

	float xsc;
	float gc;

	float gs = compressor_gain_calc_smoothing(comp, xdb, &xsc, &gc);

	// make-up gain
	float M = comp->makeup_gain;
	if (!comp->makeup_property_mode) { // auto mode
		float xsc0 = 0.0f;
		//float compressor_gain_calc_smoothing(COMPRESSOR *comp, float xdb, float *xscOut, float * gcOut);

		compressor_gain_calc_smoothing(comp, 0.0f, &xsc0, 0);
		M = -xsc0;
	}
	comp->gs = gs;
	float gm = gs + M;
	// linear gain
#if TRY_FAST
	float glin = fastPow10(gm / 20.0f);
#else
	float glin = powf(10.0, gm / 20.0f);
#endif
	comp->compress_out = glin * input;
	return comp->compress_out;
}

float compressor_gain_calc_smoothing(COMPRESSOR *comp, float xdb, float *xscOut, float * gcOut) {
	float xsc;
	if (comp->hard_knee || comp->knee < 0.1f) {  //
		if (xdb < comp->threshold) {
			xsc = xdb;
		} else {
			xsc = comp->threshold + (xdb - comp->threshold) * comp->inv_ratio;
		}
	} else {
		if (xdb < (comp->threshold - comp->knee / 2.0f)) {
			xsc = xdb;
		} else if (xdb > (comp->threshold + comp->knee / 2.0f)) {
			xsc = comp->threshold + (xdb - comp->threshold) * comp->inv_ratio;
		} else {
			xsc = xdb
					+ (1.0f - comp->inv_ratio)
							* SQUARED(xdb - comp->threshold + comp->knee / 2.0f)
							/ ( 2.0f * comp->knee);
		}
	}
	if (xscOut) *xscOut = xsc;
	float gc = xsc - xdb;
	if (gcOut) *gcOut = gc;
	// gain smoothing
	float gs;
	if (gc <= comp->gs) {
		gs = comp->alphaA * comp->gs + (1.0f - comp->alphaA) * gc;
	} else {
		gs = comp->alphaR * comp->gs + (1.0 - comp->alphaR) * gc;
	}
	return gs;
}

void setRelease_COMPRESSOR(COMPRESSOR *comp, float release_time) {
	release_time = MIN_MAX(release_time, COMPRESSOR_MINRELEASE_SEC, COMPRESSOR_MAXRELEASE_SEC);
	comp->release_time = release_time;
	comp->alphaR = EXP(-ln9 * comp->sample_time / release_time);
}
void setAttack_COMPRESSOR(COMPRESSOR *comp, float attack_time) {
	attack_time = MIN_MAX(attack_time, COMPRESSOR_MINATTACK_SEC, COMPRESSOR_MAXATTACK_SEC);
	comp->attack_time = attack_time;
	comp->alphaA = EXP(-ln9 * comp->sample_time / attack_time);
}

void setTreshold_COMPRESSOR(COMPRESSOR * comp, float threshold_db) {
	threshold_db = MIN_MAX(threshold_db, COMPRESSOR_MINTHRESHOLD_DB, COMPRESSOR_MAXTHRESHOLD_DB);
	comp->threshold = threshold_db;
}
void setKneeWidth_COMPRESSOR(COMPRESSOR * comp, float knee_width) {
	knee_width = MIN_MAX(knee_width, COMPRESSOR_MINKNEEWIDTH, COMPRESSOR_MAXKNEEWIDTH);
	comp->knee = knee_width;
}
void setMakeupGainDB_COMPRESSOR(COMPRESSOR * comp, float makeup_gain) {
	makeup_gain = MIN_MAX(makeup_gain, COMPRESSOR_MINMAKEUPGAIN_DB , COMPRESSOR_MAXMAKEUPGAIN_DB );
	comp->makeup_gain = makeup_gain;
}

