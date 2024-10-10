/*
 * first_order_allpass.c
 *
 *  Created on: Oct 7, 2024
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

#include "first_order_allpass.h"

/*
 * first_order_all_pass.c
 *
 *  Created on: Oct 7, 2024
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

#include <math.h>
#include "components/effects_macros.h"

static const float PI = 3.14159265358979f;

void initialize_FIRSTORDERALLPASSFILTER(FIRSTORDERALLPASSFILTER *apf, float centerFreq,
		float sampleRate) {
	apf->sampleTime = 1.0/sampleRate;
	apf->sampleRate = sampleRate;
	setCenterFrequency_FIRSTORDERALLPASSFILTER(apf, centerFreq);
	apf->delay_n_1 = 0;
}

float update_FIRSTORDERALLPASSFILTER(FIRSTORDERALLPASSFILTER *apf, float input) {
	apf->apfOut = apf->a1 * input + apf->delay_n_1;
	apf->delay_n_1 = input - apf->a1 * apf->apfOut;
	return apf->apfOut;
}

void setCenterFrequency_FIRSTORDERALLPASSFILTER(FIRSTORDERALLPASSFILTER *apf, float centerFreq) {
	centerFreq = MIN_MAX (centerFreq, 0.01f, 0.5 * apf->sampleRate);
	apf->tanF = tanf(PI*centerFreq * apf->sampleTime);
	apf->a1 = (apf->tanF - 1.0f) / (apf->tanF + 1.0f);
}


