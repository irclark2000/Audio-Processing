/*
 * circular_buffer.h
 *
 *  Created on: May 24, 2022
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

#ifndef EFFECTS_COMPONENTS_CIRCULAR_BUFFER_H_
#define EFFECTS_COMPONENTS_CIRCULAR_BUFFER_H_
#include "fsl_common.h"

typedef struct {
	float *storage;
	int32_t rd_ptr;
	int32_t wr_ptr;
	uint32_t size;
	uint32_t count;
} CIRCBUFFER;

uint16_t cb_transferInFloat(CIRCBUFFER *cb, float input);
uint32_t cb_transferIn(CIRCBUFFER *cb, float * source, uint32_t size);
float cb_transferOut(CIRCBUFFER *cb);
uint16_t cb_blockTransferOut(CIRCBUFFER *cb, float * output, uint32_t count);
void cb_initialize(CIRCBUFFER *cb, float * buf, uint32_t size);
//float cb_getFloatAtReadPtrWithIndex(CIRCBUFFER *cb, uint32_t index);
//float cb_getFloatAtIndex(CIRCBUFFER *cb, int32_t index);
void cb_setFloatAtWritePointer(CIRCBUFFER *cb, float value);

#endif /* EFFECTS_COMPONENTS_CIRCULAR_BUFFER_H_ */
