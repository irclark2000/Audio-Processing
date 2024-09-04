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
#include "circular_buffer.h"

uint32_t cb_transferInFloat(CIRCBUFFER *cb, float input) {
	if (cb->count < cb->size) {
		cb->storage[cb->wr_ptr++] = input;
		if (cb->wr_ptr == cb->size) cb->wr_ptr = 0;
		cb->count++;
		return 1;
	}
	else {
		return 0;
	}
}
float cb_transferOut(CIRCBUFFER *cb) {
	if (cb->count > 0) {
		float value = cb->storage[cb->rd_ptr];
		cb->rd_ptr++;
		if (cb->rd_ptr == cb->size) cb->rd_ptr = 0;
		cb->count--;
		return value;
	}
	else return 0.0f;
}

// using memcpy: transferring four bytes per float!!
// faster than using a for loop
uint16_t cb_blockTransferOut(CIRCBUFFER *cb, float * output, uint32_t count) {
	if (count > cb->count) {
		return 0;
	}
	else {
		if (cb->rd_ptr + count > cb->size) {
			uint32_t transferCount = cb->size - cb->rd_ptr;
			if (transferCount) {
				memcpy(output, &(cb->storage[cb->rd_ptr]), transferCount * 4);
			}
			memcpy(output + transferCount, cb->storage, (count-transferCount) * 4);
		}
		else {
			memcpy(output, (cb->storage + cb->rd_ptr), count * 4);
		}
		cb->rd_ptr = (cb->rd_ptr + count) % cb->size;
		cb->count -= count;
		return 1;
	}
}

float cb_getFloat (CIRCBUFFER *cb, uint32_t index) {
	index = (cb->rd_ptr + index) % cb->size;
	return cb->storage[index];
}

uint32_t cb_transferIn(CIRCBUFFER *cb, float * source, uint32_t count) {
	if (count + cb->count <= cb->size) {
		if (cb->wr_ptr + count < cb->size) {  // all in one
			memcpy(cb->storage + cb->wr_ptr, source, count * sizeof(float));
		}
		else {
			uint32_t count1 = cb->size - cb->wr_ptr;
			memcpy(cb->storage + cb->wr_ptr, source, count1 * sizeof(float));
			uint32_t count2 = count - count1;
			memcpy(cb->storage, source + count1, count2 * sizeof(float));
		}
		cb->count += count;
		cb->wr_ptr = (cb->wr_ptr + count) % cb->size;
		return count;
	}
	else { // not enough room;
		return 0;
	}

}
uint32_t cb_transferOutWithHop(CIRCBUFFER *cb, float * dest, uint32_t hop, uint32_t size) {

}
void cb_initialize(CIRCBUFFER *cb, float * buf, uint32_t size) {
	cb->storage = buf;
	cb->rd_ptr = 0;
	cb->wr_ptr = 0;
	cb->size = size;
	cb->count = 0;
	memset(buf, 0, size * sizeof(float));
}
