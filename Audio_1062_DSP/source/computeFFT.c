/*
 * computeFFT.c
 *
 *  Created on: Aug 25, 2024
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

#include "computeFFT.h"
#include <cr_section_macros.h>
#include "circular_buffer.h"

CIRCBUFFER cbBufIn;
CIRCBUFFER cbBufOut;
#define WINDOW_LENGTH 64
#define HOP 64
arm_rfft_fast_instance_f32 fft_instance;
__NOINIT(RAM3)  float32_t g_FFTAudioOut[2*(FFT_LENGTH + WINDOW_LENGTH)];
__NOINIT(RAM3)  float32_t g_FFTAudioIn[FFT_LENGTH + WINDOW_LENGTH];
__NOINIT(RAM3)  float32_t g_FFTInput[FFT_LENGTH];
__NOINIT(RAM3)  float32_t g_FFTOutput[FFT_LENGTH];
__NOINIT(RAM3)  float32_t g_FFTMag[FFT_LENGTH/2];
__NOINIT(RAM3)  float32_t g_FFTPhase1[FFT_LENGTH/2];
__NOINIT(RAM3)  float32_t g_FFTPhase2[FFT_LENGTH/2];

//  allows retaining a previous set of phase information for phase vocoder calc
float * s_FFTPhase_current;
float * s_FFTPhase_previous;

// used by arm_atan2_f32() not sure why it is undefined
int __signbitf(float x) { // @suppress("Name convention for function")
  return ((uint32_t) x & 0x80000000) >> 31; // @suppress("Avoid magic numbers")
}

void intializeFFT(void) {
	arm_status status = ARM_MATH_SUCCESS;
	status = arm_rfft_fast_init_f32(&fft_instance, (unsigned)WINDOW_LENGTH);
	cb_initialize(&cbBufIn, g_FFTAudioIn, FFT_LENGTH + WINDOW_LENGTH);
	cb_initialize(&cbBufOut, g_FFTAudioOut, FFT_LENGTH + WINDOW_LENGTH);
	s_FFTPhase_current = g_FFTPhase1;
	s_FFTPhase_previous = g_FFTPhase2;
}

void phaseVocoder (void * bufferOut, uint16_t size) {
	while(cb_transferOutWithHop(&cbBufIn, g_FFTInput, HOP, WINDOW_LENGTH)) {
		arm_rfft_fast_f32(&fft_instance, g_FFTInput, g_FFTOutput, 0);
		// calculate magnitude and phase of FFT
		arm_cmplx_mag_f32(g_FFTOutput, g_FFTMag, WINDOW_LENGTH/2);
		float32_t * destination = s_FFTPhase_current;
		for (int i=0; i < WINDOW_LENGTH/2; ++i) {
			arm_atan2_f32(g_FFTOutput[2*i+1], g_FFTOutput[2*i], destination);
			destination++;
		}
		// reconstruct FFTOutput from magnitude and phase
		for (int i=0; i < WINDOW_LENGTH/2; ++i) {
			g_FFTOutput[2*i]   = g_FFTMag[i] * arm_cos_f32(s_FFTPhase_current[i]);
			g_FFTOutput[2*i+1] = g_FFTMag[i] * arm_sin_f32(s_FFTPhase_current[i]);
		}
		arm_rfft_fast_f32(&fft_instance, g_FFTOutput, g_FFTInput, 1);
		cb_addOverlap(&cbBufOut, g_FFTInput, HOP, WINDOW_LENGTH);
	}
	int16_t *bufOut = (int16_t *) bufferOut;
	// fill the output buffer with overlap add results
	if (cbBufOut.count >= size / 4) {
		for(int i=0; i < size/2; i += 2) {
			int16_t value = (int) (cb_transferOut(&cbBufOut) * 32768.0f);
			bufOut[i] = value;
			bufOut[i+1] = value;
		}
	}
	else {
		memset(bufferOut, 0, size);
	}
}
void computeFFT(void * bufferOut, uint16_t size) {
	cb_blockTransferOut(&cbBufIn, g_FFTInput, FFT_LENGTH);
	arm_rfft_fast_f32(&fft_instance, g_FFTInput, g_FFTOutput, 0);
	// calculate magnitude and phase of FFT
	arm_cmplx_mag_f32(g_FFTOutput, g_FFTMag, FFT_LENGTH/2);
	float32_t * destination = s_FFTPhase_current;
	for (int i=0; i < FFT_LENGTH/2; ++i) {
		arm_atan2_f32(g_FFTOutput[2*i+1], g_FFTOutput[2*i], destination);
		destination++;
	}
	// reconstruct FFTOutput from magnitude and phase
	for (int i=0; i < FFT_LENGTH/2; ++i) {
		g_FFTOutput[2*i]   = g_FFTMag[i] * arm_cos_f32(s_FFTPhase_current[i]);
		g_FFTOutput[2*i+1] = g_FFTMag[i] * arm_sin_f32(s_FFTPhase_current[i]);
	}
	if (s_FFTPhase_current == g_FFTPhase1) {
		s_FFTPhase_current = g_FFTPhase2;
		s_FFTPhase_previous = g_FFTPhase1;
	}
	else {
		s_FFTPhase_current = g_FFTPhase1;
		s_FFTPhase_previous = g_FFTPhase2;
	}
	// after this call, the inverse data is in g_FFTInput
	arm_rfft_fast_f32(&fft_instance, g_FFTOutput, g_FFTInput, 1);
	int16_t *bufOut = (int16_t *) bufferOut;
	// fill the output buffer with the inverse FFT results
	for(int i=0; i < size/2; i += 2) {
		int16_t value = (int) (g_FFTInput[i/2] * 32768.0f);  // convert to ints
		bufOut[i] = value;
		bufOut[i+1] = value;
	}
}
