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

#include "asymmetric_overdrive.h"

#include <fast_math/fast_math.h>

#define EXP(x) fastExp(x)

static float OVERDRIVE_INPUT_LPF_COEF[OVERDRIVE_INPUT_LPF_LENGTH] = {
  0.0017537939320576863, 0.007340938089113929, 0.010684659857249475,
  0.0059465192664006445, -0.0042426166630165525, -0.006997638595382362,
  0.002026157681616623, 0.008949616068073098, 0.0008912997218693121,
  -0.010828116840199064, -0.005341942938835928, 0.011629118328725593,
  0.011319672204048182, -0.010437864918854862, -0.01855507417411213,
  0.006264099063997508, 0.026534382918826812, 0.002082360459837173,
  -0.034558207485434624, -0.016429660182211446, 0.04186999401097673,
  0.04105973748393176, -0.04773119656100209, -0.09174975610860517,
  0.051520616641375305, 0.31338828169865396, 0.4471688410545182,
  0.31338828169865396, 0.051520616641375305, -0.09174975610860517,
  -0.04773119656100209, 0.04105973748393176, 0.04186999401097673,
  -0.016429660182211446, -0.034558207485434624, 0.002082360459837173,
  0.026534382918826812, 0.006264099063997508, -0.01855507417411213,
  -0.010437864918854862, 0.011319672204048182, 0.011629118328725593,
  -0.005341942938835928, -0.010828116840199064, 0.0008912997218693121,
  0.008949616068073098, 0.002026157681616623, -0.006997638595382362,
  -0.0042426166630165525, 0.0059465192664006445, 0.010684659857249475,
  0.007340938089113929, 0.0017537939320576863
};


void initialize_ASYMMETRIC_OVERDRIVE(ASYMMETRIC_OVERDRIVE *aod, float samplingFreq, float hpfCutoffFreq,
		float preGain, float lpfOutCutoffFreq, float lpfOutDamp) {
	initialize_OVERDRIVE(&(aod->od), samplingFreq, hpfCutoffFreq, preGain, lpfOutCutoffFreq, lpfOutDamp);
	aod->Q = -0.2;
}
float update_ASYMMETRIC_OVERDRIVE(ASYMMETRIC_OVERDRIVE *aod, float input) {
	OVERDRIVE *od = &(aod->od);
	// Input FIR low pass filter (anti-aliasing)
	od->lpfInpBuf[od->lpfInpBufIndex] = input;
	od->lpfInpBufIndex++;

	if (od->lpfInpBufIndex == OVERDRIVE_INPUT_LPF_LENGTH) {
		od->lpfInpBufIndex = 0;
	}

	od->lpfInpOut = 0.0f;

	uint32_t index = od->lpfInpBufIndex;
	for(int i=0; i < OVERDRIVE_INPUT_LPF_LENGTH; ++i) {
		if (index == 0) {
			index = OVERDRIVE_INPUT_LPF_LENGTH - 1;
		}
		else {
			index--;
		}
		od->lpfInpOut += OVERDRIVE_INPUT_LPF_COEF[i] * od->lpfInpBuf[index];
	}

	// IIR Input high pass filter to remove some low frequencies that muddy the output
	od->hpfBufInp[1] = od->hpfBufInp[0];
	od->hpfBufInp[0] = od->lpfInpOut;

	od->hpfBufOut[1] = od->hpfBufOut[0];
	od->hpfBufOut[0] = (2.0f * (od->hpfBufInp[0] - od->hpfBufInp[1]) + (2.0f - od->hpfInpWct) * od->hpfBufOut[1]) / (2.0f + od->hpfInpWct);
	od->hpfInpOut = od->hpfBufOut[0];

	// Overdrive non linear clipping
#if 0
	float clipInp = od->preGain * od->hpfInpOut;
	float absClipInp = fabs(clipInp);
	float sgnClip = (clipInp > 0.0f) ? 1.0f : -1.0f;
	float clipOut = 0.0f;
	if (absClipInp < od->threshold) {
		clipOut = 2.0 * clipInp;
	} else if (absClipInp < 2.0 * od->threshold) {
		clipOut = sgnClip * (3.0f - (2.0 - 3.0 * absClipInp) * (2.0 - 3.0 * absClipInp)) / 3.0f;
	} else {
		clipOut = sgnClip;
	}
#endif

	/* Asymmetrical clipping */
	float xGain = od->preGain * od->hpfInpOut;
	static const float d =  8.0f;

	float clipOut = aod->Q / (1.0f - EXP(d * aod->Q));

	if ((xGain - aod->Q) >= 0.00001f) {

		clipOut += (xGain - aod->Q) / (1.0f - EXP(-d * (xGain - aod->Q)));

	}

	// Output 2nd order low pass filter to remove some harmonic components after clipping.
	od->lpfOutBufIn[2] = od->lpfOutBufIn[1];
	od->lpfOutBufIn[1] = od->lpfOutBufIn[0];
	od->lpfOutBufIn[0] = clipOut;
	od->lpfOutBufOut[2] = od->lpfOutBufOut[1];
	od->lpfOutBufOut[1] = od->lpfOutBufOut[0];
	od->lpfOutBufOut[0] =
			od->coefIIR.b0 * od->lpfOutBufIn[0] + od->coefIIR.b1 * od->lpfOutBufIn[1] + od->coefIIR.b2 * od->lpfOutBufIn[2] -
			od->coefIIR.a1 * od->lpfOutBufOut[1] - od->coefIIR.a2 * od->lpfOutBufOut[2];
	//od->lpfOutWct;
	//od->lpfOutDamp;
	od->lpfOutOut = od->lpfOutBufOut[0];

	od->out = od->lpfOutOut;

	// final clamp
	if (od->out > 1.0f) {
		od->out = 1.0f;
	} else if (od->out < -1.0f) {
		od->out = -1.0f;
	}
	aod->out = od->out;
	return aod->out;
}
