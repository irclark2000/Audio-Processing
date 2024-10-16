/*
 * process.h
 *
 *  Created on: Oct 13, 2024
 *      Author: isaac
 */

#ifndef TESTER_PROCESS_H_
#define TESTER_PROCESS_H_

#include <stdint.h>

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
