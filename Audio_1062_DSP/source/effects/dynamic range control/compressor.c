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

#include <dynamic range control/compressor.h>
static float log9 = 0.95424250943f;

float compressor_gain_calc_smoothing(COMPRESSOR *comp, float xdb, float *xscOut);

void initialize_compressor(COMPRESSOR *comp, float sample_rate) {
	comp->sample_time = 1.0f / sample_rate;
	comp->gs = 0.0f;
	comp->inv_ratio = 1.0f / 5.0f;
	comp->threshold = -10.0f;  //db
	comp->knee = 0.0f;
	comp->makeup_gain = 0.0f;
	comp->makeup_property_mode = 1;
	comp->compress_out = 0.0f;
	comp->hard_knee = 0;
	compressor_setRelease(comp, 0.2f);
	compressor_setAttack(comp, 0.05f);
}
// based on MATLAB compressor

float update_compressor(COMPRESSOR *comp, float input) {
	float xdb = 20.0f * logf(input);

	float xsc;
	float gs = compressor_gain_calc_smoothing(comp, xdb, &xsc);
	// make-up gain
	float M = comp->makeup_gain;
	if (!comp->makeup_property_mode) { // auto mode
		float xsc0;
		compressor_gain_calc_smoothing(comp, 0.0f, &xsc0);
		M = -xsc0;
	}
	float gm = gs + M;
	// linear gain
	float glin = powf(10.0, gm / 20.0f);
	comp->compress_out = glin * input;
	return comp->compress_out;
}

float compressor_gain_calc_smoothing(COMPRESSOR *comp, float xdb, float *xscOut) {
	float xsc;
	if (comp->hard_knee) {  //
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
							* (xdb - comp->threshold + comp->knee / 2.0f)
							* (xdb - comp->threshold + comp->knee / 2.0f) / 2.0f
							/ comp->knee;
		}
	}
	*xscOut = xsc;
	float gc = xsc - xdb;
	// gain smoothing
	if (gc <= comp->gs) {
		comp->gs = comp->alphaA * comp->gs + (1.0f - comp->alphaA) * gc;
	} else {
		comp->gs = comp->alphaR * comp->gs + (1.0 - comp->alphaR) * gc;
	}
	return gc;
}

void compressor_setRelease(COMPRESSOR *comp, float release_time) {
	comp->release_time = release_time;
	comp->alphaR = expf(-log9 * comp->sample_time / release_time);
}
void compressor_setAttack(COMPRESSOR *comp, float attack_time) {
	comp->attack_time = attack_time;
	comp->alphaA = expf(-log9 * comp->sample_time / attack_time);
}
