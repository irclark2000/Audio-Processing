/*
 * rms_compressor.c
 *
 *  Created on: Jul 4, 2025
 *      Author: isaac
 */
/*Copyright 2025 Isaac R. Clark, Jr.

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

#include "rms_compressor.h"
#include <fast_math/fast_math.h>
#include "components/effects_macros.h"

#define TRY_FAST 0
#if TRY_FAST
#define EXP fastExp
#define LOG10 fastLog10
#else
#define EXP expf
#define LOG10 log10f
#endif

void initialize_RMSCOMPRESSOR (RMS_COMPRESSOR * rms_comp, float sample_rate) {
	COMPRESSOR *comp = &(rms_comp->compressor);
	RMS *rms = &(rms_comp->rms);
	initialize_COMPRESSOR (comp, sample_rate);
	initialize_RMS(rms, sample_rate);
}

float update_RMSCOMPRESSOR (RMS_COMPRESSOR * rms_comp, float input) {
	COMPRESSOR *comp = &(rms_comp->compressor);
	RMS *rms = &(rms_comp->rms);
	comp->absInput = apply_RMS(rms, input);
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
