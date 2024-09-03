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

arm_rfft_fast_instance_f32 fft_instance;
__NOINIT(RAM3)  float32_t g_FFTInput[FFT_LENGTH];
__NOINIT(RAM3)  float32_t g_FFTOutput[FFT_LENGTH];
__NOINIT(RAM3)  float32_t g_FFTMag[FFT_LENGTH/2];
void intializeFFT(void) {
	arm_status status = ARM_MATH_SUCCESS;
	status = arm_rfft_fast_init_f32(&fft_instance, (unsigned)FFT_LENGTH);
}
void computeFFT(void) {
	arm_rfft_fast_f32(&fft_instance, g_FFTInput, g_FFTOutput, 0);
	arm_cmplx_mag_f32(g_FFTOutput, g_FFTMag, FFT_LENGTH/2);
	arm_rfft_fast_f32(&fft_instance, g_FFTOutput, g_FFTInput, 1);
}
