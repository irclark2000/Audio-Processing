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


#include "effects/pitch_shift.h"
#include "effects/tremolo.h"
#include "filters/equalizing_filter.h"
#include "reverbs/schroeder_verb.h"
#include "reverbs/freeverb.h"
#include "phase vocoder/computeFFT.h"
#include "delay based/echo.h"
#include "delay based/flanger.h"
#include "delay based/chorus.h"
#include "dynamic range control/expander.h"
#include "dynamic range control/compressor.h"
#include "dynamic range control/limiter.h"
#include "process.h"
#include "updateSettings.h"
#include "random_generator.h"
#include "overdrive.h"
#include "dynamic range control/noise_gate.h"
#include "fast_math.h"

//#include "overdrive.h"

#include <cr_section_macros.h>


#ifndef PHASEVOCODER
#define PHASEVOCODER 0
#endif

// Take care if enabling more than one at a time
#define TESTING_CHORUS 0
#define TESTING_FLANGER 0
#define TESTING_ECHO 0
#define TESTING_EQUALIZER 0
#define TESTING_PITCH_CHANGE 0
#define TESTING_LIMITER 0
#define TESTING_EXPANDER 0
#define TESTING_COMPRESSOR 1
#define TESTING_NOISEGATE 0
#define TESTING_OVERDRIVE 0
#define TESTING_TREMOLO 0
#define TESTING_FREEVERB 0
#define TESTING_SCHROEDER 0
#define TESTING_NOISE_GENERATOR 0
#define TESTING_PHASE_VOCODER 0

#if TESTING_NOISEGATE
NOISEGATE nGate;
#endif

const float INT16_TO_FLOAT = 1.0f / 32768.0f;
#if TESTING_ECHO
ECHO echo;
#define ECHO_BUF_SIZE 4096
__NOINIT(RAM3) static float echo_buf[ECHO_BUF_SIZE];
#endif
#if TESTING_FLANGER
FLANGER flanger;
#define FLANGER_BUF_SIZE 4096
__NOINIT(RAM3) static float flanger_buf[FLANGER_BUF_SIZE];
#endif

#if TESTING_CHORUS
#define CHORUS_BUF_SIZE 4096
#define CHORUS_ELEMENT_COUNT 2
__NOINIT(RAM3) static float chorus_buf0[CHORUS_BUF_SIZE];
__NOINIT(RAM3) static float chorus_buf1[CHORUS_BUF_SIZE];

CHORUS chorus;
CHORUSELEMENT cE0;
CHORUSELEMENT cE1;

#endif
#if TESTING_TREMOLO
TREMOLO trem;
#endif
#if TESTING_PITCH_CHANGE
HIGHPASS hp;
PITCHSHIFT ps;
#endif
#if TESTING_FREEVERB
FREEVERB fvb;
#endif
#if TESTING_SCHROEDER
FREEVERB fvb;
SCHROEDERVERB svb;
#endif
#if TESTING_OVERDRIVE
OVERDRIVE od;
#endif
#if TESTING_EQUALIZER
	EQFILTER eqf0;
	EQFILTER eqf1;
	EQFILTER eqf2;
	EQFILTER eqf3;
#endif
#if TESTING_LIMITER
LIMITER limiter;
#endif
#if TESTING_EXPANDER
	EXPANDER expander;
#endif
#if TESTING_COMPRESSOR
	COMPRESSOR compressor;
#endif

void initializeEffects(float sampleRate) {
#if TESTING_TREMOLO
	Tremolo_Init(&trem, 0.55f, 220.0f, sampleRate);
#endif
#if TESTING_ECHO
	intialize_ECHO (&echo, echo_buf, ECHO_BUF_SIZE, 80.0f, 0.3f, 0.5f, 0.5f, sampleRate);
#endif
#if TESTING_CHORUS
	chorus.cElement[0] = cE0;
	chorus.cElement[1] = cE1;
	// (freq, depth)
	CELEMENTDATA cData[] = {
			(0.01f, 0.01f), (0.02f,0.03f)
	};

	float *buffer_array [] = {
		chorus_buf1, chorus_buf1
	};

	initialize_CHORUS (&chorus, CHORUS_ELEMENT_COUNT, buffer_array, 0.5f,
			CHORUS_BUF_SIZE, 20.0f, cData, sampleRate);
#endif
#if TESTING_FREEVERB
	initFreeverb(&fvb);
#endif

#if TESTING_SCHROEDER
	initSchroederVerb(&svb);
#endif
#if TESTING_EQUALIZER
	intitialize_random_number_generator();
	EQFILTER_initialize(&eqf0, 4000.0f, sampleRate, 10.0f, 200.0f);
	EQFILTER_initialize(&eqf1, 12000.0f, sampleRate, 10.0f, 200.0f);
	EQFILTER_initialize(&eqf2, 12000.0f, sampleRate, 1.0f, 200.0f);
	EQFILTER_initialize(&eqf3, 500.0f, sampleRate, 1.0f, 200.0f);
#endif
#if TESTING_PHASE_VOCODER
	intializeFFT();
#endif
#if TESTING_FLANGER
	intialize_FLANGER (&flanger, flanger_buf, FLANGER_BUF_SIZE,
			80.0f, 0.25f, 30.0f,
			0.4f, 0.5, sampleRate);
#endif
#if TESTING_LIMITER
	initialize_LIMITER(&limiter,  sampleRate);
#endif
#if TESTING_EXPANDER
	initialize_EXPANDER (&expander, sampleRate);
#endif
#if TESTING_COMPRESSOR
	initialize_COMPRESSOR(&compressor,  sampleRate);
#endif
#if TESTING_OVERDRIVE
	// distortion level of 50 yields high distortion.
	initialize_OVERDRIVE(&od, sampleRate, 30.0f, 10.0f, 6400.0f, 0.6);
#endif
#if TESTING_NOISEGATE
	initializeNoiseGate(&nGate, 10.0f, 1.0f, 10.0f, sampleRate, 0.1f);
#endif
#if TESTING_PITCH_CHANGE
	initHighPass(&hp,
				0.9862117951198142f,
				-1.9724235902396283f,
				0.9862117951198142f,
				-1.972233470205696f,
				0.9726137102735608f);
		initPitchShift(&ps, &hp, pitch_buf, PITCH_BUFFER_SIZE, 0.25f, 1.0f);
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
#if TESTING_NOISE_GENERATOR
		rightIn = 0.05 * get_random_float(); // make white noise
#endif
		leftOut = rightIn;
#if !TESTING_PHASE_VOCODER
#if TESTING_CHORUS
		leftOut = update_CHORUS(&chorus, rightIn);
#endif

		//leftOut = 1.4 * g_gain * arm_sin_f32(i * 10.0 / sampleRate);
		//leftOut = 1.05f * Tremolo_Update(&trem, leftIn, false);
		//rightOut = 1.05f * Tremolo_Update(&trem, rightIn, true);
		//leftOut = EQFILTER_update(&eqf0, rightIn);
#if TESTING_EQUALIZER
		// test EQ filter using freq scan over random noise
		rightIn = 0.05 * get_random_float();
		float filterOut = EQFILTER_update(&eqf0, rightIn);
		filterOut = EQFILTER_update(&eqf1, filterOut);
		filterOut = EQFILTER_update(&eqf2, filterOut);
		leftOut = EQFILTER_update(&eqf3, filterOut);
#endif
#if TESTING_FLANGER
		leftOut = update_FLANGER (&flanger, rightIn);
#endif
#if TESTING_ECHO
		leftOut = update_Echo (&echo, rightIn);
#endif
#if TESTING_NOISEGATE
		leftOut = 1.5f * applyNoiseGate(&nGate, rightIn);
#endif
#if TESTING_SCHROEDER
		leftOut = 3.0 * g_gain * applyShroederVerb(&svb, rightIn);
#endif
#if TESTING_FREEVERB
		leftOut = applyFreeverb(&fvb, rightIn);
#endif
#if TESTING_PITCH_CHANGE
		leftOut = 1.5 * applyPitchShift(&ps, rightIn);
#endif
#if TESTING_OVERDRIVE
		leftOut = 3.0 * update_OVERDRIVE(&od, rightIn);
#endif
#if TESTING_TREMOLO
		leftOut = 1.05f * Tremolo_Update(&trem, rightIn, true);
#endif
#if TESTING_LIMITER
	leftOut = update_LIMITER(&limiter,  rightIn);
#endif
#if TESTING_EXPANDER
	leftOut = update_EXPANDER(&expander,  rightIn);
#endif
#if TESTING_COMPRESSOR
	leftOut = update_COMPRESSOR(&compressor,  rightIn);
#endif

		rightOut = leftOut;
		bufOut[i]   = (int) (leftOut * 32768.0f);
		bufOut[i+1] = (int) (rightOut * 32768.0f);
#elif TESTING_PHASE_VOCODER
		transferInFloat_FFTCIRCBUFFER(&FFTcBufIn, rightIn);
#endif
	} // for loop
	//memcpy(bufOut, bufIn, byteCount);
#if TESTING_PHASE_VOCODER
	phaseVocoder (bufferOut, size);
#endif
}

const static float inverse_time = 1.0f / 799.0f;
#define USE_FAST_APPROX 1
// apply automatic variation to a parameter
void test_PROCESS (uint32_t update_counter) {
	float param = update_counter * inverse_time;
	//delay parameter over about 8 seconds
//	setDelayMSec_ECHO (&echo, getMaxDelayMS_ECHO(&echo) * param);
	//setFeedback_level_ECHO (&echo, 0.95 * param);
#if TESTING_FLANGER
	setFeedback_level_FLANGER(&flanger, 0.95f * param);
#endif
#if TESTING_EQUALIZER
	// frequency sweep  over 4.6 octaves
#if USE_FAST_APPROX
	float freq0 = 500 * fastPow2 (4.6f * (update_counter * inverse_time));
	float freq1 = 500 * fastPow2 (4.6f * (1.0f - update_counter * inverse_time));
#else
	float freq0 = 500 * powf(2.0f, 4.6f * (update_counter * inverse_time));
	float freq1 = 500 * powf(2.0f, 4.6f * (1.0f - update_counter * inverse_time));
#endif
	EQFILTER_setCenterFrequency(&eqf0, freq0, 200.0f);
	EQFILTER_setCenterFrequency(&eqf3, freq0, 200.0f);
	EQFILTER_setCenterFrequency(&eqf1, freq1, 200.0f);
	EQFILTER_setCenterFrequency(&eqf2, freq1, 200.0f);
	//EQFILTER_setGain (&eqf0, 10.0 * update_counter * inverse_time);
#endif
}
