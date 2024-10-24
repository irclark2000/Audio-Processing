/*
 * process.h
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

#ifndef HARDWARE_SPECIFIC_PROCESS_H_
#define HARDWARE_SPECIFIC_PROCESS_H_
//#include "fsl_common.h"
#include <stdint.h>
#if AUDIO_EFFECTS_TESTER
#include "compatibility_macros/compatibility.h"
#endif
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef PHASEVOCODER
#define PHASEVOCODER 0
#endif
typedef enum {
	MONO = 1,
	STEREO = 2,
	UNKNOWN
} AUDIOFORMAT;

void processHalf(const void *bufferIn, void *bufferOut,
		uint32_t frameCount, AUDIOFORMAT audioFmt,
		float sampleRate);
void initializeEffects(float sampleRate);
void test_PROCESS (uint32_t update_counter);

#if defined(__cplusplus)
}
#endif

#endif /* HARDWARE_SPECIFIC_PROCESS_H_ */
