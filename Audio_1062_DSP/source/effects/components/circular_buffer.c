/*
 * circular_buffer.h
 *
 *  Created on: May 24, 2022
 *      Author: isaac
 *
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
#include <components/circular_buffer.h>
#if AUDIO_EFFECTS_TESTER
#include <stdlib.h>
#endif
#include "memory_debug.h"

uint16_t cb_transferInFloat(CIRCBUFFER *cb, float input) {
		cb->storage[cb->wr_ptr++] = input;
		if ((uint32_t)cb->wr_ptr == cb->size)
			cb->wr_ptr = 0;
		if (cb->count < cb->size) {
			cb->count++;
		}
		return 1;
}
float cb_transferOut(CIRCBUFFER *cb) {
	if (cb->count > 0) {
		float value = cb->storage[cb->rd_ptr];
		cb->rd_ptr++;
		if ((uint32_t)cb->rd_ptr == cb->size)
			cb->rd_ptr = 0;
		cb->count--;
		return value;
	} else
		return 0.0f;
}

// using memcpy: transferring four bytes per float!!
// faster than using a for loop
const uint16_t bytesPerFloat = sizeof(float);

uint16_t cb_blockTransferOut(CIRCBUFFER *cb, float *output, uint32_t count) {
	if (count > cb->count) {
		return 0;
	} else {
		if (cb->rd_ptr + count > cb->size) {
			uint32_t transferCount = cb->size - cb->rd_ptr;
			if (transferCount) {
				memcpy(output, &(cb->storage[cb->rd_ptr]),
						transferCount * bytesPerFloat);
			}
			memcpy(output + transferCount, cb->storage,
					(count - transferCount) * bytesPerFloat);
		} else {
			memcpy(output, (cb->storage + cb->rd_ptr), count * bytesPerFloat);
		}
		cb->rd_ptr = (cb->rd_ptr + count) % cb->size;
		cb->count -= count;
		return 1;
	}
}
#if 0
float cb_getFloatAtReadPtrWithIndex(CIRCBUFFER *cb, uint32_t index) {
	index = (cb->rd_ptr + index + 5 * cb->size) % cb->size;
	return cb->storage[index];
}
#endif
#if 0
float cb_getFloatAtIndex(CIRCBUFFER *cb, int32_t index) {
	index = (index + 5 * cb->size) % cb->size;
	return cb->storage[index];
}
#endif
#if 0
void cb_setFloatAtWritePointer(CIRCBUFFER *cb, float value) {
	cb->storage[cb->wr_ptr] = value;
}
#endif
uint32_t cb_transferIn(CIRCBUFFER *cb, float *source, uint32_t count) {
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
#if 0
// returns 0 if there is not sufficient data in the buffer
// fills destination buffer and then advances the read pointer by the hop size
uint16_t cb_transferOutWithHop(CIRCBUFFER *cb, float *dest, uint32_t hop_size,
		uint32_t window_size) {
	uint32_t rd_ptr_save = cb->rd_ptr;
	uint32_t count_save = cb->count;
	uint16_t success = cb_blockTransferOut(cb, dest, window_size);
	if (success) {
		cb->count = count_save - hop_size;
		cb->rd_ptr = (rd_ptr_save + hop_size) % cb->size;
	}
	return success;
}
uint16_t cb_addOverlap(CIRCBUFFER *cb, float *buf, uint32_t hop_size,
		uint32_t window_size) {
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
#endif
void gui_cb_initialize(CIRCBUFFER *cb, float maxDelaySec, float sampleRate) {
	uint32_t size = maxDelaySec * sampleRate;
	cb_initialize(cb, 0, size);
}
void cb_initialize(CIRCBUFFER *cb, float *buf, uint32_t size) {
#if AUDIO_EFFECTS_TESTER
	if (buf == 0) {
		buf = (float *) MALLOC(size * sizeof(float));
	}

#endif
	cb->storage = buf;
	cb->rd_ptr = 0;
	cb->wr_ptr = 0;
	cb->size = size;
	cb->count = 0;
	memset(buf, 0, size * bytesPerFloat);
}
void cb_uninitialize(CIRCBUFFER *cb) {
#if AUDIO_EFFECTS_TESTER
	FREE(cb->storage);
#endif
}
