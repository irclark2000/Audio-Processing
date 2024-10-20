/*
 * process.c
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

#if AUDIO_EFFECTS_TESTER
#include "tester_process.h"
#include "apply_effect.h"

static const float S16_TO_FLOAT = 1.0f / 32768.0f; // supported
static const float S24_TO_FLOAT = 1.0f / (8388608.0f);
static const float S32_TO_FLOAT = 1.0f / (2147483648.0f);
static const float F32_TO_FLOAT = 1.0f;

void initializeEffects(float sampleRate) {
	initializeExternalEffect(0, sampleRate);
}

void processHalf(const void *bufferIn, void *bufferOut,
		uint32_t frameCount, AUDIOFORMAT audioFmt,
		AUDIO_DATA_TYPE dataFmt, float sampleRate) {
	static float leftIn, rightIn;
	static float leftOut, rightOut;
	const int16_t *bufIn;    // use 16 bit pointers to match the word size
	int16_t *bufOut;

	if (dataFmt != S16) return;

	uint8_t channelCount;
	uint32_t loopCount;
	if (audioFmt == MONO) {
		channelCount = 1;
	}else if (audioFmt == STEREO) {
		channelCount = 2;
	}
	else {
		return;
	}
    loopCount = frameCount / channelCount;
	bufIn =  (int16_t*) bufferIn;
	bufOut = (int16_t*) bufferOut;

	for (int i = 0; i < loopCount; i += channelCount) {
		leftIn = bufIn[i] * S16_TO_FLOAT;
		// convert S16 data to float and deal with MONO/STEREO
		if (audioFmt == MONO) {
			rightIn = leftIn;
		}
		else {
			rightIn = bufIn[i + 1] * S16_TO_FLOAT;
		}
		// no effect code
		leftOut = leftIn;
		rightOut = rightIn;

		leftOut = applyExternalEffectCode (0, leftIn);
		rightOut = leftOut;

		bufOut[i] = (int16_t)(leftOut * 32768.0f);
		bufOut[i+1] = (int16_t)(rightOut * 32768.0f);
	}

}

void test_PROCESS (uint32_t update_counter) {

}
#endif //AUDIO_EFFECTS_TESTER
