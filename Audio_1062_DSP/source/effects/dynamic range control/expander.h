/*
 * expander.h
 *
 *  Created on: Sep 18, 2024
 *      Author: isaac
 */
/*
Copyright 2024 Isaac R. Clark, Jr.

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
#ifndef EFFECTS_DYNAMIC_RANGE_CONTROL_EXPANDER_H_
#define EFFECTS_DYNAMIC_RANGE_CONTROL_EXPANDER_H_

#include <stdint.h>
#include <math.h>

#define EXPANDER_MAXTHRESHOLD_DB 0
#define EXPANDER_MINTHRESHOLD_DB -140

typedef struct {
	float sample_time;
	float gs;  //db
	float ratio;
	float threshold;  //db
	float knee;
	float absInput;
	float attack_time;
	float release_time;
	float hold_time;
	float alphaA, alphaR;
	float hold_time_counter;
	uint8_t hard_knee;
	float expand_out;
} EXPANDER;

void initialize_EXPANDER (EXPANDER * ex, float sample_rate);
float update_EXPANDER (EXPANDER * ex, float input);
void expander_setRelease(EXPANDER * ex, float release_time);
void expander_setAttack(EXPANDER * ex, float attack_time);
void expander_setHold(EXPANDER * ex, float hold_time);
void setTreshold_EXPANDER(EXPANDER * ex, float threshold_db);

#endif /* EFFECTS_DYNAMIC_RANGE_CONTROL_EXPANDER_H_ */
