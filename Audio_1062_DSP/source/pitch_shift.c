/*
 * pitch_shift.c
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

#include <cr_section_macros.h>
#include "pitch_shift.h"

__NOINIT(RAM3) float pitch_buf[PITCH_BUFFER_SIZE];
static int round (float value);

void initPitchShift(PITCHSHIFT *ps, HIGHPASS *hp, float *buf, int size, float shift, float crossfade) {
	ps->WrtPtr = 0;
	ps->Rd_p = 0.0f;
	ps->Shift = shift;
	ps->CrossFade = crossfade;
	ps->size = size;
	ps->buffer = buf;
	ps->hp = hp;
}

float applyPitchShift(PITCHSHIFT *ps, float input) {
	ps->sum = applyHighPass(ps->hp, input);
	ps->buffer[ps->WrtPtr] = ps->sum;

	ps->RdPtr1 = round(ps->Rd_p);
	ps->RdPtr2 = 0;
	if (ps->RdPtr1 >= ps->size/2) {
		ps->RdPtr2 = ps->RdPtr1 - ps->size/2;
	}
	else {
		ps->RdPtr2 = ps->RdPtr1 + ps->size/2;
	}
	ps->Rd0 = ps->buffer[ps->RdPtr1];
	ps->Rd1 = ps->buffer[ps->RdPtr2];
	//Check if first readpointer starts overlap with write pointer?
	// if yes -> do cross-fade to second read-pointer
	if (PITCH_OVERLAP >= (ps->WrtPtr-ps->RdPtr1) && (ps->WrtPtr-ps->RdPtr1) >= 0 && ps->Shift!=1.0f) {
		int rel = ps->WrtPtr-ps->RdPtr1;
		ps->CrossFade = ((float)rel)/(float)PITCH_OVERLAP;
	}
	else if (ps->WrtPtr-ps->RdPtr1 == 0) ps->CrossFade = 0.0f;

	//Check if second readpointer starts overlap with write pointer?
	// if yes -> do cross-fade to first read-pointer
	if (PITCH_OVERLAP >= (ps->WrtPtr-ps->RdPtr2) && (ps->WrtPtr-ps->RdPtr2) >= 0 && ps->Shift!=1.0f) {
			int rel = ps->WrtPtr-ps->RdPtr2;
			ps->CrossFade = 1.0f - ((float)rel)/(float)PITCH_OVERLAP;
	}
	else if (ps->WrtPtr-ps->RdPtr2 == 0) ps->CrossFade = 1.0f;


	//do cross-fading and sum up
	ps->sum = (ps->Rd0*ps->CrossFade + ps->Rd1*(1.0f-ps->CrossFade));

	//increment fractional read-pointer and write-pointer
	ps->Rd_p += ps->Shift;
	ps->WrtPtr++;
	if (ps->WrtPtr == ps->size) ps->WrtPtr = 0;
	if (round(ps->Rd_p) >= ps->size) ps->Rd_p = 0.0f;

	return ps->sum;

	return input;
}

static int round (float value) {
	if (value < 0) {
		return (int)(value - 0.5);
	}
	else return (int)(value + 0.5);
}
