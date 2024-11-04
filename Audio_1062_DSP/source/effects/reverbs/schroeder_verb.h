/*
 * schroeder_verb.h
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


#ifndef SCHROEDER_VERB_H_
#define SCHROEDER_VERB_H_

#if !AUDIO_EFFECTS_TESTER
#include <cr_section_macros.h>
#endif

#include <reverbs/schroeder_verb.h>


#include <filters/all_pass1.h>
#include <filters/comb_filter1.h>

typedef struct {
	ALLPASS1 allpass[3];
	COMB_FILTER lpfc[4];
	int lpfcCount;
	int allCount;
	double out;
	double lpTotal;
	double wet;
	int i;

} SCHROEDERVERB;

void initSchroederVerb(SCHROEDERVERB * fv, float sampleRate);
float applyShroederVerb(SCHROEDERVERB*fv, float input);



#endif /* SCHROEDER_VERB_H_ */
