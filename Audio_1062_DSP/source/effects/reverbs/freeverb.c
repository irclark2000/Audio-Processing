/*
 * freeverb.c
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


#include <reverbs/freeverb.h>
#include <stdint.h>

#define COMB0 1557 * 48 / 44
#define COMB1 1617 * 48 / 44
#define COMB2 1491 * 48 / 44
#define COMB3 1422 * 48 / 44
#define COMB4 1277 * 48 / 44
#define COMB5 1356 * 48 / 44
#define COMB6 1188 * 48 / 44
#define COMB7 1116 * 48 / 44

#define AP0   225 * 48 / 44
#define AP1   556 * 48 / 44
#define AP2   441 * 48 / 44
#define AP3   341 * 48 / 44

AT_NONCACHEABLE_SECTION_ALIGN(static float comb0_buf[COMB0], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static float comb1_buf[COMB1], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static float comb2_buf[COMB2], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static float comb3_buf[COMB3], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static float comb4_buf[COMB4], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static float comb5_buf[COMB5], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static float comb6_buf[COMB6], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static float comb7_buf[COMB7], 4);

AT_NONCACHEABLE_SECTION_ALIGN(static float ap0_buf[AP0], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static float ap1_buf[AP1], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static float ap2_buf[AP2], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static float ap3_buf[AP3], 4);

void initFreeverb(FREEVERB * fv) {

	fv->allCount = sizeof(fv->allpass) / sizeof(fv->allpass[0]);
	fv->lpfcCount = sizeof(fv->lpfc) / sizeof(fv->lpfc[0]);

	initAllpassFilter(&(fv->allpass[0]), &(ap0_buf[0]), AP0, 0.5f);
	initAllpassFilter(&(fv->allpass[1]), &(ap1_buf[0]), AP1, 0.5f);
	initAllpassFilter(&(fv->allpass[2]), &(ap2_buf[0]), AP2, 0.5f);
	initAllpassFilter(&(fv->allpass[3]), &(ap3_buf[0]), AP3, 0.5f);

	initCombFilter(&(fv->lpfc[0]), &(comb0_buf[0]), COMB0, 0.2f, 0.84f);
	initCombFilter(&(fv->lpfc[1]), &(comb1_buf[0]), COMB1, 0.2f, 0.84f);
	initCombFilter(&(fv->lpfc[2]), &(comb2_buf[0]), COMB2, 0.2f, 0.84f);
	initCombFilter(&(fv->lpfc[3]), &(comb3_buf[0]), COMB3, 0.2f, 0.84f);
	initCombFilter(&(fv->lpfc[4]), &(comb4_buf[0]), COMB4, 0.2f, 0.84f);
	initCombFilter(&(fv->lpfc[5]), &(comb5_buf[0]), COMB5, 0.2f, 0.84f);
	initCombFilter(&(fv->lpfc[6]), &(comb6_buf[0]), COMB6, 0.2f, 0.84f);
	initCombFilter(&(fv->lpfc[7]), &(comb7_buf[0]), COMB7, 0.2f, 0.84f);


}
float applyFreeverb(FREEVERB *fv, float input) {
	fv->out = 0;
	// all in parallel
	for (fv->i = 0; fv->i < fv->lpfcCount; fv->i++) {
		fv->out += (applyCombFilter(&(fv->lpfc[fv->i]), input) * 0.125f);
	}
	// in series
	for (fv->i = 0; fv->i < fv->allCount; fv->i++) {
		fv->out = applyAllpassFilter(&(fv->allpass[fv->i]), fv->out);
	}

	return fv->out;
}
