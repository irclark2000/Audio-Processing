/*
 * process.c
 *
 *  Created on: Oct 13, 2024
 *      Author: isaac
 */

#if AUDIO_EFFECTS_TESTER
#include "tester_process.h"
static const float S16_TO_FLOAT = 1.0f / 32768.0f; // supported
static const float S24_TO_FLOAT = 1.0f / (8388608.0f);
static const float S32_TO_FLOAT = 1.0f / (2147483648.0f);
static const float F32_TO_FLOAT = 1.0f;

void initializeEffects(float sampleRate) {

}

void processHalf(const void *bufferIn, void *bufferOut,
		uint32_t frameCount, AUDIOFORMAT audioFmt,
		float sampleRate, AUDIO_DATA_TYPE dataFmt) {
	static float leftIn, rightIn;
	static float leftOut, rightOut;
	const float *bufIn;
	float *bufOut;

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
	bufIn = (float*) bufferIn;
	bufOut = (float*) bufferOut;

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



		bufOut[i] = leftOut * 32768.0f;
		bufOut[i+1] = rightOut * 32768.0f;
	}

}

void test_PROCESS (uint32_t update_counter) {

}
#endif //AUDIO_EFFECTS_TESTER
