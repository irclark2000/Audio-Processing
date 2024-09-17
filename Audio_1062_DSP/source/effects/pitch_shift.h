/*
 * pitch_shift.h
 *
 *  Created on: May 11, 2022
 *      Author: isaac
 */
/*
Copyright 2022 Isaac R. Clark, Jr.

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

#ifndef PITCH_SHIFT_H_
#define PITCH_SHIFT_H_

#include "high_pass.h"

#define PITCH_BUFFER_SIZE 1000
#define PITCH_OVERLAP 100

extern float pitch_buf[PITCH_BUFFER_SIZE];

typedef struct {
	float *buffer;
	int WrtPtr;
	int RdPtr1;
	int RdPtr2;
	float Rd_p;
	float Shift;
	float CrossFade;
	float out;
	int size;
	float sum;
	float Rd0, Rd1;
	HIGHPASS *hp;
}PITCHSHIFT;

void initPitchShift(PITCHSHIFT *ps, HIGHPASS *hp, float *buf, int size, float shift, float crossfade);
float applyPitchShift(PITCHSHIFT *ps, float input);

#endif /* PITCH_SHIFT_H_ */
