/*
 * overdrive.h
 *
 *  Created on: Aug 29, 2024
 *      Author: isaac
 *      Based on code written by Philip Salmony
 *      See YouTube Phil's Lab Episode #117
  */

#ifndef OVERDRIVE_H_
#define OVERDRIVE_H_

#include <stdint.h>
#include <math.h>

#define USE_DSP_FIR 0  // using DSP version is not feasible

#if defined USE_DSP_FIR && USE_DSP_FIR == 1
#include "arm_math.h"
#endif

#define OVERDRIVE_INPUT_LPF_LENGTH 53
//extern const float OVERDRIVE_INPUT_LPF_COEF[OVERDRIVE_INPUT_LPF_LENGTH];
typedef struct {
	float a0;
	float a1;
	float a2;
	float b0;
	float b1;
	float b2;
}IIRSecondOrder;

typedef struct {

	float T;  // sampling period

	// Input FIR low pass filter
	float lpfInpOut;

#if defined USE_DSP_FIR && USE_DSP_FIR == 1
	arm_fir_instance_f32 fir_instance;
#else
	float lpfInpBuf[OVERDRIVE_INPUT_LPF_LENGTH];
	uint32_t lpfInpBufIndex;
#endif

	// Input high pass filter
	float hpfBufInp[2];
	float hpfBufOut[2];
	float hpfInpWct;
	float hpfInpOut;
	float gui_HPFFreq;

	// Settings
	float preGain;
	float threshold;

	// Output IIR low pass filter
	float lpfOutBufIn[3];
	float lpfOutBufOut[3];
	float lpfOutWct;
	float lpfOutDamp;
	float lpfOutOut;
	float gui_LPFFreq;

	IIRSecondOrder coefIIR;

	float out;

} OVERDRIVE;


void initialize_OVERDRIVE(OVERDRIVE *od, float samplingFreq, float hpfCutoffFreq,
		float preGain, float lpfOutCutoffFreq, float lpfOutDamp);
void overdriveSetLPF(OVERDRIVE *od, float cutoffFreq, float damping);
void overdriveSetHPF(OVERDRIVE *od, float cutoffFreq);
float update_OVERDRIVE(OVERDRIVE *od, float input);
void gui_overdriveSetLPF(OVERDRIVE *od);

void gui_overdriveSetHPF(OVERDRIVE *od);



#endif /* OVERDRIVE_H_ */
