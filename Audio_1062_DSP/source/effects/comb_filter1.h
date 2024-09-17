/*
 * comb_filter1.h
 *
 *  Created on: May 13, 2022
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

#ifndef COMB_FILTER1_H_
#define COMB_FILTER1_H_

typedef struct {
	float gain;
	float *buf;
	float bufLimit;
	int bufSize;
	int bufIndx;
	float out;
	float next;
	int i;

} COMB_FILTER;

void initCombFilter1(COMB_FILTER * comb, float *buffer, int size, float gain);
void upDateCombFilter1(COMB_FILTER * comb, float limitFactor);
float applyCombFilter1(COMB_FILTER * comb, float input);


#endif /* COMB_FILTER1_H_ */
