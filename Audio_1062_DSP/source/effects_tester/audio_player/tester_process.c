/*
 * process.c
 *
 *  Created on: Oct 13, 2024
 *      Author: isaac
 */

#if AUDIO_EFFECTS_TESTER
#include "tester_process.h"

void initializeEffects(float sampleRate) {

}

void processHalf(const void *bufferIn, void *bufferOut,
		uint32_t frameCount, AUDIOFORMAT audioFmt,
		float sampleRate) {
	static float leftIn, rightIn;
	static float leftOut, rightOut;
	const float *bufIn;
	float *bufOut;

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
		leftIn = bufIn[i];
		if (audioFmt == MONO) {
			rightIn = leftIn;
		}
		else {
			rightIn = bufIn[i + 1];
		}
		// no effect code
		leftOut = leftIn;
		rightOut = rightIn;



		bufOut[i] = leftOut;
		bufOut[i+1] = rightOut;
	}

}

void test_PROCESS (uint32_t update_counter) {

}
#endif //AUDIO_EFFECTS_TESTER
