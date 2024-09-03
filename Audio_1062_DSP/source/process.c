/*
 * process.c
 *
 *  Created on: Apr 16, 2022
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


#include "process.h"
#include "tremolo.h"
#include "freeverb.h"
#include "schroeder_verb.h"
#include "pitch_shift.h"
#include "noise_gate.h"
#include "computeFFT.h"
#include "updateSettings.h"
#include "arm_math.h"
#include "overdrive.h"
TREMOLO trem;
FREEVERB fvb;
SCHROEDERVERB svb;
HIGHPASS hp;
PITCHSHIFT ps;
NOISEGATE nGate;
OVERDRIVE od;

const float INT16_TO_FLOAT = 1.0f / 32768.0f;

void initializeEffects(float sampleRate) {
	Tremolo_Init(&trem, 0.55f, 220.0f, sampleRate);
	initFreeverb(&fvb);
	initSchroederVerb(&svb);
	intializeFFT();

	// distortion level of 50 yields high distortion.
	overdriveInit(&od, sampleRate, 30.0f, 10.0f, 6400.0f, 0.6);
	initializeNoiseGate(&nGate, 10.0f, 1.0f, 10.0f, sampleRate, 0.1f);
#if 1
	initHighPass(&hp,
				0.9862117951198142f,
				-1.9724235902396283f,
				0.9862117951198142f,
				-1.972233470205696f,
				0.9726137102735608f);
		initPitchShift(&ps, &hp, pitch_buf, PITCH_BUFFER_SIZE, 1.5f, 1.0f);
#endif

}
// size is the number of bytes in a half buffer but we will be using
// 16 bit integer values so two bytes per sample  half left and half right
void processHalf(void *bufferIn, void *bufferOut, uint16_t size, float sampleRate) {
	int16_t * bufIn;    // use 16 bit pointers to match the word size
	int16_t * bufOut;
	static float leftIn, rightIn;
	static float leftOut, rightOut;

	bufIn = (int16_t *) bufferIn;
	bufOut = (int16_t *) bufferOut;

	for (int i=0; i < size/2; i+= 2) {
		leftIn = bufIn[i] * INT16_TO_FLOAT ;
		rightIn = bufIn[i+1] * INT16_TO_FLOAT;
		leftOut = rightIn;

//		leftOut = 1.4 * g_gain * arm_sin_f32(i * 10.0 / sampleRate);
//		leftOut = 1.05f * Tremolo_Update(&trem, leftIn, false);
//		rightOut = 1.05f * Tremolo_Update(&trem, rightIn, true);
		//leftOut = 3.0 * g_gain * applyShroederVerb(&svb, rightIn);
		leftOut = applyPitchShift(&ps, rightIn);
		leftOut = 1.5f * applyNoiseGate(&nGate, rightIn);
		leftOut = applyFreeverb(&fvb, rightIn);
		//leftOut = 3.0 * overdriveUpdate(&od, rightIn);
		leftOut = 1.05f * Tremolo_Update(&trem, rightIn, true);
		leftOut = 3.0 * g_gain * applyShroederVerb(&svb, rightIn);
		//leftOut = rightIn;
		rightOut = leftOut;
		bufOut[i]   = (int) (leftOut * 32768.0f);
		bufOut[i+1] = (int) (rightOut * 32768.0f);
		g_FFTInput[i/2] = rightIn;
	}
	//memcpy(bufOut, bufIn, byteCount);
	computeFFT();
}

