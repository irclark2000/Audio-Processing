/*
 * compressor.h
 *
 *  Created on: Sep 19, 2024
 *      Author: isaac
 */
/*Copyright 2024 Isaac R. Clark, Jr.

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
#ifndef EFFECTS_DYNAMIC_RANGE_CONTROL_COMPRESSOR_H_
#define EFFECTS_DYNAMIC_RANGE_CONTROL_COMPRESSOR_H_

#include <stdint.h>
#include <math.h>

typedef struct {
	float sample_time;
	float gs;  //db
	float inv_ratio;
	float gui_ratio;
	float threshold;  //db
	float makeup_gain; //db
	float knee;
	float absInput;
	float attack_time;
	float release_time;
	float alphaA, alphaR;
	uint8_t hard_knee;
	int gui_not_hard_knee;
	int gui_not_hard_property;
	uint8_t makeup_property_mode;
	float compress_out;
} COMPRESSOR;

void initialize_COMPRESSOR (COMPRESSOR * comp, float sample_rate);
float update_COMPRESSOR (COMPRESSOR * comp, float input);
void setRelease_COMPRESSOR(COMPRESSOR * comp, float release_time);
void setAttack_COMPRESSOR(COMPRESSOR * comp, float attack_time);
void gui_setAttackRelease_COMPRESSOR(COMPRESSOR *comp);
void gui_setInverseRatio_COMPRESSOR(COMPRESSOR *comp);
void setKneeWidth_COMPRESSOR(COMPRESSOR * comp, float knee_width);
void setMakeupDBGain_COMPRESSOR(COMPRESSOR * comp, float makeup_gain);
void gui_setHardKnee_COMPRESSOR (COMPRESSOR *comp);
void gui_setMakeupPropertyKnee_COMPRESSOR (COMPRESSOR *comp);
#endif /* EFFECTS_DYNAMIC_RANGE_CONTROL_COMPRESSOR_H_ */
