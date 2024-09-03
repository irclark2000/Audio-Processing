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

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_
#include "fsl_common.h"

typedef struct {
	float *storage;
	uint32_t rd_ptr;
	uint32_t end_ptr;
	uint32_t size;
	uint32_t count;
} CIRC_BUFFER;

uint32_t cb_transferInFloat(CIRC_BUFFER *cb, float input);
uint32_t cb_transferIn(CIRC_BUFFER *cb, float * source, uint32_t size);
uint32_t cb_transferOutWithHop(CIRC_BUFFER *cb, float * dest, uint32_t hop, uint32_t size);
void cb_initialize(CIRC_BUFFER *cb, float * buf, uint32_t size);
float cb_getFloat (CIRC_BUFFER *cb, uint32_t index);

#endif /* CIRCULAR_BUFFER_H_ */
