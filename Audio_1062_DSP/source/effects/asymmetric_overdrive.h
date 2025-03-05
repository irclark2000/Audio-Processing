/*
 * asymmetric_overdrive.h
 *
 *  Created on: Mar 4, 2025
 *      Author: isaac
 *
 *      Based on code written by Philip Salmony
 *      See YouTube Phil's Lab Episode #153
*/
/*
Copyright 2025 Isaac R. Clark, Jr.

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

#ifndef EFFECTS_ASYMMETRIC_OVERDRIVE_H_
#define EFFECTS_ASYMMETRIC_OVERDRIVE_H_

#include "overdrive.h"

typedef struct ASYMMETRIC_OVERDRIVE {
	OVERDRIVE od;
	float postGain;
	float Q;
	float out;
} ASYMMETRIC_OVERDRIVE;

void initialize_ASYMMETRIC_OVERDRIVE(ASYMMETRIC_OVERDRIVE *aod, float samplingFreq, float hpfCutoffFreq,
		float preGain, float lpfOutCutoffFreq, float lpfOutDamp);
float update_ASYMMETRIC_OVERDRIVE(ASYMMETRIC_OVERDRIVE *aod, float input);


#endif /* EFFECTS_ASYMMETRIC_OVERDRIVE_H_ */
