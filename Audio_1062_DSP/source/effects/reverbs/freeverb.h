/*
 * freeverb.h
 *
 *  Created on: Apr 9, 2022
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

#ifndef FREEVERB_H_
#define FREEVERB_H_

#if AUDIO_EFFECTS_TESTER
#include "effects_tester/audio_player/effect_component.h"
#else
#include <fsl_common.h>
#endif
#include <filters/all_pass.h>
#include <filters/comb_filter.h>

typedef struct {
	ALLPASS allpass[4];
	LPFCOMB_FILTER lpfc[8];
	int lpfcCount;
	int allCount;
	double out;
	double lpTotal;
	int i;

} FREEVERB;

void initFreeverb(void* vfv, float sampleRate);
float applyFreeverb(void *vfv, float input);
void uninitialize_Freeverb(void *vfv);
#if AUDIO_EFFECTS_TESTER
void initialize_FREEVERB(void *vfv, EFFECT_PARAMS *params, float sampleRate);
EFFECT_COMPONENT * initializeComponent_Freeverb (FREEVERB *fv, EFFECT_COMPONENT *component);
#endif
#endif /* FREEVERB_H_ */
