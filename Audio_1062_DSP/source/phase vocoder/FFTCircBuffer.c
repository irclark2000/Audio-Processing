/*
 * FFTCircBuffer.c
 *
 *  Created on: Sep 26, 2024
 *      Author: isaac
 *      Wrapper around Circular Buffer
 */

#include "FFTCircBuffer.h"
#include <string.h>
static const uint16_t bytesPerFloat = sizeof(float);


void initialize_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb, float * buf, uint32_t size) {
	cb_initialize(&(fcb->cBuf), buf, size);
}

uint16_t addOverlap_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb, float * buf, uint32_t hop_size, uint32_t window_size) {
	CIRCBUFFER *cb = &(fcb->cBuf);
	uint32_t wr_ptr_save = cb->wr_ptr;
	uint32_t count_save = cb->count;
	uint32_t overlap = window_size - hop_size;
	if (cb->count + hop_size > cb->size) { // check for room
		return 0;
	}
	// overlap add the overlapping part, but just copy the non-overlapping part.
	for (int i = 0; i < overlap; ++i) { // overlap add
		uint32_t index = (cb->wr_ptr + i) % cb->size;
		cb->storage[index] += buf[i];
	}
	// final copying of the non-overlapped portion
	cb->wr_ptr = (wr_ptr_save + overlap) % cb->size;
	cb_transferIn(cb, buf + overlap, hop_size);
	cb->count = count_save + hop_size;
	cb->wr_ptr = (wr_ptr_save + hop_size) % cb->size;
	return 1;
}

uint32_t transferIn_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb, float * source, uint32_t count) {
	CIRCBUFFER *cb = &(fcb->cBuf);
	if (count + cb->count <= cb->size) {
		if (cb->wr_ptr + count < cb->size) {  // all in one
			if (source) {
				memcpy(cb->storage + cb->wr_ptr, source, count * bytesPerFloat);
			}
			else {
				memset(cb->storage + cb->wr_ptr, 0, count * bytesPerFloat);
			}
		} else {
			uint32_t count1 = cb->size - cb->wr_ptr;
			if (source) {
				memcpy(cb->storage + cb->wr_ptr, source, count1 * bytesPerFloat);
			}
			else {
				memset(cb->storage + cb->wr_ptr, 0, count1 * bytesPerFloat);
			}
			uint32_t count2 = count - count1;
			if (source) {
				memcpy(cb->storage, source + count1, count2 * bytesPerFloat);
			}
			else {
				memset(cb->storage, 0, count2 * bytesPerFloat);
			}
		}
		if (cb->count + count <= cb->size) {
		cb->count += count;
		}
		else {
			cb->count = cb->size;
		}
		cb->wr_ptr = (cb->wr_ptr + count) % cb->size;
		return count;
	} else { // not enough room;
		return 0;
	}
}

// returns 0 if there is not sufficient data in the buffer
// fills destination buffer and then advances the read pointer by the hop size
uint16_t transferOutWithHop_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb, float * dest, uint32_t hop_size, uint32_t window_size) {
	CIRCBUFFER *cb = &(fcb->cBuf);
	uint32_t rd_ptr_save = cb->rd_ptr;
	uint32_t count_save = cb->count;
	uint16_t success = cb_blockTransferOut(cb, dest, window_size);
	if (success) {
		cb->count = count_save - hop_size;
		cb->rd_ptr = (rd_ptr_save + hop_size) % cb->size;
	}
	return success;
}
uint16_t blockTransferOut_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb, float *output, uint32_t count) {
	return cb_blockTransferOut(&(fcb->cBuf), output, count);
}
float transferOut_FFTCIRCBUFFER(FFTCIRCBUFFER *fcb) {
	cb_transferOut(&(fcb->cBuf));
}
