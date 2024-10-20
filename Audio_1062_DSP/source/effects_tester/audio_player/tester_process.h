/*
 * process.h
 *
 *  Created on: Oct 13, 2024
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

#ifndef TESTER_PROCESS_H_
#define TESTER_PROCESS_H_

#include <stdint.h>
#include "apply_effect.h"

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef PHASEVOCODER
#define PHASEVOCODER 0
#endif
typedef enum {
	MONO =   1,
	STEREO = 2,
	TRIO =   3,
	QUAD =   4,
	UNKNOWN
} AUDIOFORMAT;

typedef enum {
	Unknown = 0,
	U8, //  | 8-bit unsigned integer                 | [0, 255]                 |
	S16, // | 16-bit signed integer                  | [-32768, 32767]           |
	S24, // | 24-bit signed integer (tightly packed) | [-8388608, 8388607]       |
	S32, // | 32-bit signed integer                  | [-2147483648, 2147483647] |
	F32  // | 32-bit floating point                  | [-1, 1]                   |
} AUDIO_DATA_TYPE;

void processHalf(const void *bufferIn, void *bufferOut,
		uint32_t frameCount,
		AUDIOFORMAT audioFmt,
		AUDIO_DATA_TYPE dataFmt,
		float sampleRate);
void initializeEffects(float sampleRate);
void test_PROCESS (uint32_t update_counter);

#if defined(__cplusplus)
}
#endif

#endif /* TESTER_PROCESS_H_ */
