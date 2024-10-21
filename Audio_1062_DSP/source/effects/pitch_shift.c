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

#if !AUDIO_EFFECTS_TESTER
#include <cr_section_macros.h>
#else
#include "compatibility_macros/compatibility.h"
#endif
#include "pitch_shift.h"

// as written, we can only have one instance of pitch shift.
// the buffer would have to be moved outside.
#if AUDIO_EFFECTS_TESTER
#include <stdlib.h>
#else
__NOINIT(RAM3) float pitch_buf[PITCH_BUFFER_SIZE];
#endif

static int roundOff (float value);
HIGHPASS hp;


void initPitchShift(PITCHSHIFT *ps, float *buf, int size, float shift, float crossfade) {
#if AUDIO_EFFECTS_TESTER
	if (buf == 0) {
		buf = (float *) malloc(int * sizeof(float));
	}
#endif
	ps->WrtPtr = 0;
	ps->Rd_p = 0.0f;
	ps->Shift = shift;
	ps->CrossFade = crossfade;
	ps->size = size;
	ps->buffer = buf;
	initHighPass(&hp,
				0.9862117951198142f,
				-1.9724235902396283f,
				0.9862117951198142f,
				-1.972233470205696f,
				0.9726137102735608f);
}

float applyPitchShift(PITCHSHIFT *ps, float input) {
	ps->sum = applyHighPass(&(ps->hp), input);
	ps->buffer[ps->WrtPtr] = ps->sum;

	ps->RdPtr1 = roundOff(ps->Rd_p);
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
	if (roundOff(ps->Rd_p) >= ps->size) ps->Rd_p = 0.0f;

	return ps->sum;

	return input;
}
void setPitchShift(PITCHSHIFT *ps, float potentiometer) {

}
static int roundOff (float value) {
	if (value < 0) {
		return (int)(value - 0.5);
	}
	else return (int)(value + 0.5);
}
