/*
 * rms_calculator.c
 *
 *  Created on: Jul 3, 2025
 *      Author: isaac
 */
/*
Copyright 2025 Isaac R. Clark, Jr.

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
#include <math.h>
#include "rms_calculator.h"


void initialize_RMS (RMS * rms, float sampleRate) {
	rms->gui_window_length_mSec = 50.0f;
	rms->X_n_1_2 = 0.0f;
	rms->Y_n_1_2 = 0.0f;
	rms->out = 0.0f;
	rms->sampleRate = sampleRate;
	set_window_mSec_RMS (rms, 50.0f);
}
float apply_RMS (RMS * rms, float input) {
	rms->input_squared = input * input;
	rms->Y_n_1_2 = rms->Y_n_1_2 + rms->L_inv * (rms->input_squared - rms->X_n_1_2);
	rms->X_n_1_2 = rms->input_squared;
	rms->out = sqrt(rms->Y_n_1_2);
	return 0.0f;
}
void set_window_mSec_RMS (RMS *rms, float m_sec) {
	rms->gui_window_length_mSec = m_sec;
	gui_set_window_mSec_RMS (rms);
}
void gui_set_window_mSec_RMS (RMS *rms) {
	rms->L_inv = 1.0f/(rms->sampleRate * rms->gui_window_length_mSec/1000.0f);
}
void gui_initialize_RMS (RMS * rms, float sampleRate) {
	rms->X_n_1_2 = 0.0f;
	rms->Y_n_1_2 = 0.0f;
	rms->out = 0.0f;
	rms->sampleRate = sampleRate;
	set_window_mSec_RMS (rms, rms->gui_window_length_mSec);
}

