/*
 * schroeder_verb.c
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

#include "schroeder_verb.h"
#define MULT 2

#define SCOMB0 3460 * 44 / 96 * MULT
#define SCOMB1 2988 * 44 / 96 * MULT
#define SCOMB2 3882 * 44 / 96 * MULT
#define SCOMB3 4312 * 44 / 96 * MULT

#define SAP0 480 * 44 / 96 * MULT
#define SAP1 161 * 44 / 96 * MULT
#define SAP2 46  * 44 / 96 * MULT

__NOINIT(RAM3) static float comb0_buf[SCOMB0];
__NOINIT(RAM3) static float comb1_buf[SCOMB1];
__NOINIT(RAM3) static float comb2_buf[SCOMB2];
__NOINIT(RAM3) static float comb3_buf[SCOMB3];

__NOINIT(RAM3) static float ap0_buf[SAP0];
__NOINIT(RAM3) static float ap1_buf[SAP1];
__NOINIT(RAM3) static float ap2_buf[SAP2];


void initSchroederVerb(SCHROEDERVERB * fv) {
	fv->allCount = sizeof(fv->allpass) / sizeof(fv->allpass[0]);
	fv->lpfcCount = sizeof(fv->lpfc) / sizeof(fv->lpfc[0]);

	initAllpassFilter1(&(fv->allpass[0]), &(ap0_buf[0]), SAP0, 0.7f);
	initAllpassFilter1(&(fv->allpass[1]), &(ap1_buf[0]), SAP1, 0.7f);
	initAllpassFilter1(&(fv->allpass[2]), &(ap2_buf[0]), SAP2, 0.7f);

	initCombFilter1(&(fv->lpfc[0]), &(comb0_buf[0]), SCOMB0, 0.805f);
	initCombFilter1(&(fv->lpfc[1]), &(comb1_buf[0]), SCOMB1, 0.827f);
	initCombFilter1(&(fv->lpfc[2]), &(comb2_buf[0]), SCOMB2, 0.783f);
	initCombFilter1(&(fv->lpfc[3]), &(comb3_buf[0]), SCOMB3, 0.764f);

}
float applyShroederVerb(SCHROEDERVERB*fv, float input) {
	fv->out = 0;
	// all in parallel
	for (fv->i = 0; fv->i < fv->lpfcCount; fv->i++) {
		fv->out += (applyCombFilter1(&(fv->lpfc[fv->i]), input) * 0.25f);
	}
	// in series
	for (fv->i = 0; fv->i < fv->allCount; fv->i++) {
		fv->out = applyAllpassFilter1(&(fv->allpass[fv->i]), fv->out);
	}

	return fv->out;

}
