/*
 * computeFFT.h
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

#ifndef COMPUTEFFT_H_
#define COMPUTEFFT_H_
#ifndef ARM_MATH_CM7
#define ARM_MATH_CM7
#endif


#include <math.h>
#include <arm_math.h>
#include "arm_const_structs.h"
#include "audio_wm8960.h"
#include "circular_buffer.h"


#define FFT_LENGTH HALF_BUFFER_SIZE/4
extern CIRCBUFFER cbBufIn;

#if defined(__cplusplus)
extern "C" {
#endif
void intializeFFT(void);
void phaseVocoder (void * bufferOut, uint16_t size);
void computeFFT(void * bufferOut, uint16_t size);
#if defined(__cplusplus)
}
#endif


#endif /* COMPUTEFFT_H_ */
