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

#include <dynamic range control/expander.h>
static float log9 = 0.95424250943f;

void initialize_expander(EXPANDER *ex, float sample_rate) {
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
	expander_setRelease(ex, 0.2);
	expander_setAttack(ex, 0.05);
	expander_setHold(ex, 0.05);
}
// based on MATLAB expander

float update_expander(EXPANDER *ex, float input) {
	float xdb = 20.0f * logf(input);

	float xsc;
	// calculate static characteristic
	if (ex->hard_knee) {  //
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
							* (xdb - ex->threshold - ex->knee / 2.0f);
		} else {
			xsc = xdb;
		}
		float gc = xsc = xdb;
		// gain smoothing
		// find place to increment counter and to reset counter back to zero
		if ((ex->hold_time_counter > ex->hold_time) && (gc < ex->gs)) {
			ex->gs = ex->alphaA * ex->gs + (1.0 - ex->alphaA) * gc;
		} else if (ex->hold_time_counter <= ex->hold_time) {
			ex->gs = ex->gs;
			ex->hold_time_counter += ex->sample_time;
		} else if (gc > ex->gs) {
			ex->gs = ex->alphaR * ex->gs + (1.0 - ex->alphaR) * gc;
			ex->hold_time_counter = 0;
		}
	}
	// linear gain
	float glin = powf(10.0, ex->gs / 20.0f);
	ex->expand_out = glin * input;
	return ex->expand_out;
}
static float log9;
//= logf(9.0f);

void expander_setRelease(EXPANDER *ex, float release_time) {
	ex->release_time = release_time;
	ex->alphaR = expf(-log9 * ex->sample_time / release_time);
}
void expander_setAttack(EXPANDER *ex, float attack_time) {
	ex->attack_time = attack_time;
	ex->alphaA = expf(-log9 * ex->sample_time / attack_time);
}
void expander_setHold(EXPANDER *ex, float hold_time) {
	ex->hold_time = hold_time;
}