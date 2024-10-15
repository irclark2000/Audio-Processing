/*
 * expander.c
 *
 *  Created on: Sep 18, 2024
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

#include <dynamic_range_control/expander.h>
#include <fast_math/fast_math.h>

static const float log9 = 0.95424250943f;
static const float ln9 = 2.19722457734f;

#define TRY_FAST 0
#if TRY_FAST
#define EXP fastExp
#define LOG10 fastLog10
#else
#define EXP expf
#define LOG10 log10f
#endif

#define EXPANDER_MINTHRESHOLD_DB -140
#define EXPANDER_MAXTHRESHOLD_DB 0
#define EXPANDER_MINRATIO 1
#define EXPANDER_MAXRATIO 50
#define EXPANDER_MINKNEEWIDTH 0
#define EXPANDER_MAXKNEEWIDTH 20
#define EXPANDER_MINRELEASE_SEC 0
#define EXPANDER_MAXRELEASE_SEC 4
#define EXPANDER_MINATTACK_SEC 0
#define EXPANDER_MAXATTACK_SEC 4
#define EXPANDER_MINHOLD_SEC 0
#define EXPANDER_MAXHOLD_SEC 4
#define COMPRESSOR_MINMAKEUPGAIN_DB -10
#define COMPRESSOR_MAXMAKEUPGAIN_DB 24

#include "components/effects_macros.h"

void initialize_EXPANDER(EXPANDER *ex, float sample_rate) {
	ex->sample_time = 1.0f / sample_rate;
	ex->gs = 0.0f;
	ex->ratio = 5.0f;
	ex->threshold = -10.0;  //db
	ex->knee = 0.0f;
	//ex->attack_time = 0.05f;
	//ex->release_time = 0.2f;
	//ex->hold_time = 0.05f;
	ex->hold_time_counter = 0.0f;
	ex->expand_out = 0.0f;
	ex->hard_knee = 0;
	setRelease_EXPANDER(ex, 0.2);
	setAttack_EXPANDER(ex, 0.05);
	setHold_EXPANDER(ex, 0.05);
	setTreshold_EXPANDER(ex, -50.0f);
}
// based on MATLAB expander

float update_EXPANDER(EXPANDER *ex, float input) {
	ex->absInput = input;
	if (input < 0.0f) {
		ex->absInput = -ex->absInput;
	}

	float xdb = 20.0f * LOG10(ex->absInput);
	if (isINF(xdb)) {
		return input;
	}

	float xsc;
	// calculate static characteristic
	if (ex->hard_knee || ex->knee < 0.1f) {  //
		if (xdb < ex->threshold) {
			xsc = ex->threshold + (xdb - ex->threshold) * ex->ratio;
		} else {
			xsc = xdb;
		}
	} else {
		if (xdb < (ex->threshold - ex->knee / 2.0f)) {
			xsc = ex->threshold + (xdb - ex->threshold) * ex->ratio;
		} else if (xdb > (ex->threshold + ex->knee / 2.0f)) {
			xsc = xdb
					+ (1.0f - ex->ratio)
							* SQUARED(xdb - ex->threshold - ex->knee / 2.0f);
		} else {
			xsc = xdb;
		}
	}
	float gc = xsc - xdb;
	// gain smoothing
	// find place to increment counter and to reset counter back to zero
	if ((ex->hold_time_counter > ex->hold_time) && (gc < ex->gs)) {
		ex->gs = ex->alphaA * ex->gs + (1.0 - ex->alphaA) * gc;
	} else if (gc > ex->gs) {
		ex->gs = ex->alphaR * ex->gs + (1.0 - ex->alphaR) * gc;
		ex->hold_time_counter = 0;
	} else if (ex->hold_time_counter <= ex->hold_time) {
		ex->gs = ex->gs;
		ex->hold_time_counter += ex->sample_time;
	}
	// linear gain
#if TRY_FAST
	float glin = fastPow10(ex->gs / 20.0f);
#else
	float glin = powf(10.0, ex->gs / 20.0f);
#endif
	ex->expand_out = glin * input;
	return ex->expand_out;
}

void setRelease_EXPANDER(EXPANDER *ex, float release_time) {
	release_time = MIN_MAX(release_time, EXPANDER_MINRELEASE_SEC, EXPANDER_MAXRELEASE_SEC);
	ex->release_time = release_time;
	ex->alphaR = EXP(-ln9 * ex->sample_time / release_time);
}
void setAttack_EXPANDER(EXPANDER *ex, float attack_time) {
	attack_time = MIN_MAX(attack_time, EXPANDER_MINATTACK_SEC, EXPANDER_MAXATTACK_SEC);
	ex->attack_time = attack_time;
	ex->alphaA = EXP(-ln9 * ex->sample_time / attack_time);
}
void setHold_EXPANDER(EXPANDER *ex, float hold_time) {
	hold_time = MIN_MAX(hold_time, EXPANDER_MINHOLD_SEC, EXPANDER_MAXHOLD_SEC);
	ex->hold_time = hold_time;
}
void setTreshold_EXPANDER(EXPANDER * ex, float threshold_db) {
	threshold_db = MIN_MAX(threshold_db, EXPANDER_MINTHRESHOLD_DB, EXPANDER_MAXTHRESHOLD_DB);
	ex->threshold = threshold_db;
}
void setKneeWidth_EXPANDER(EXPANDER * ex, float knee_width) {
	knee_width = MIN_MAX(knee_width, EXPANDER_MINKNEEWIDTH, EXPANDER_MAXKNEEWIDTH);
	ex->knee = knee_width;
}
