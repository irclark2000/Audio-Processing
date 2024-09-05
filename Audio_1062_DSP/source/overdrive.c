/*
 * overdrive.c
 *
 *  Created on: Aug 29, 2024
 *      Author: isaac
 *      Based on code written by Philip Salmony
 *      See YouTube Phil's Lab Episode #117
 */

#include "overdrive.h"
//#include "fsl_common.h"
/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 48000 Hz

* 0 Hz - 10000 Hz
  gain = 1
  desired ripple = 1 dB
  actual ripple = 0.6943232005924238 dB

* 12000 Hz - 24000 Hz
  gain = 0
  desired attenuation = -60 dB
  actual attenuation = -60.692689098108126 dB

*/
// calculates a1, a2, b0, b1, b2 for final high pass
void calculateIIRCoef(OVERDRIVE *od, float freq);

// FIR components 53 taps
//AT_NONCACHEABLE_SECTION_ALIGN(static float OVERDRIVE_INPUT_LPF_COEF[OVERDRIVE_INPUT_LPF_LENGTH]);
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

#if !defined USE_DSP_FIR || !USE_DSP_FIR
const float PI = 3.1415926535;
#endif


void overdriveInit(OVERDRIVE *od, float samplingFreq, float hpfCutoffFreq,
		float preGain, float lpfOutCutoffFreq, float lpfOutDamp) {
	od->T = 1.0/samplingFreq;
	// Initialize input FIR low pass filter
	for(int i=0; i < OVERDRIVE_INPUT_LPF_LENGTH; ++i) {
		od->lpfInpBuf[i] = 0.0f;
	}
	od->lpfInpBufIndex = 0;
	od->lpfInpOut = 0;

	// Initialize input high pass filter
	od->hpfBufInp[0] = 0;
	od->hpfBufInp[1] = 0;
	od->hpfBufOut[0] = 0;
	od->hpfBufOut[1] = 0;
	od->hpfInpWct = 2.0f * PI * hpfCutoffFreq * od->T;
	od->hpfInpOut = 0;

	// Settings
	od->preGain = preGain;
	od->threshold = 0.333333333f;

	// Initialize output low pass filter
	od->lpfOutBufIn[0] = 0.0f;
	od->lpfOutBufIn[1] = 0.0f;
	od->lpfOutBufIn[2] = 0.0f;
	od->lpfOutBufOut[0] = 0.0f;
	od->lpfOutBufOut[1] = 0.0f;
	od->lpfOutBufOut[2] = 0.0f;
	od->lpfOutWct = 2.0f * PI * lpfOutCutoffFreq * od->T;
	od->lpfOutDamp = lpfOutDamp;
	od->lpfOutOut = 0;
	calculateIIRCoef(od, lpfOutCutoffFreq);
	od->out = 0;
}

void overdriveSetLPF(OVERDRIVE *od, float cutoffFreq, float damping) {
	od->lpfOutWct = 2.0f * PI * cutoffFreq * od->T;
	od->lpfOutDamp = damping;
	calculateIIRCoef(od, cutoffFreq);
}

void overdriveSetHPF(OVERDRIVE *od, float cutoffFreq) {
	od->hpfInpWct = 2.0f * PI * cutoffFreq * od->T;

}
float overdriveUpdate(OVERDRIVE *od, float input) {
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

	return od->out;
}

// second order coefficient calculation
void calculateIIRCoef(OVERDRIVE *od, float freq) {
	float wc = 2.0f * PI * freq;
	float k = wc / tan(wc * od->T / 2.0f);
	float denominator = wc * wc + k * k + sqrt(2.0) * wc * k;
	od->coefIIR.a0 = 1.0f;
	od->coefIIR.a1 = 2.0f * (wc * wc - k * k) / denominator;
	od->coefIIR.a2 = (wc * wc + k * k - sqrt(2.0) * wc * k) / denominator;
	od->coefIIR.b0 = (k * k) / denominator;
	od->coefIIR.b1 = -2.0f * od->coefIIR.b0;
	od->coefIIR.b2 = od->coefIIR.b0;
}
