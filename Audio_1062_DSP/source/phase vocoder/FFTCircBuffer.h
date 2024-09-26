/*
 * FFTCircBuffer.h
 *
 *  Created on: Sep 26, 2024
 *      Author: isaac
 */

#ifndef PHASE_VOCODER_FFTCIRCBUFFER_H_
#define PHASE_VOCODER_FFTCIRCBUFFER_H_

#include "circular_buffer.h"
typedef struct {
	CIRCBUFFER cBuf;
}FFTCIRCBUFFER;

void initialize_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb, float * buf, uint32_t size);
uint16_t addOverlap_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb, float * buf, uint32_t hop_size, uint32_t window_size);
uint32_t transferIn_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb, float * source, uint32_t size);
uint16_t transferOutWithHop_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb, float * dest, uint32_t hop_size, uint32_t window_size);
uint16_t blockTransferOut_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb, float *output, uint32_t count);
float transferOut_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb);

#endif /* PHASE_VOCODER_FFTCIRCBUFFER_H_ */
