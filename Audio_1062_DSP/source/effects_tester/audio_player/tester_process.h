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

#endif /* TESTER_PROCESS_H_ */
