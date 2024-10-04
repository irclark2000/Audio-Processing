/*
 * wah_wah.c
 *
 *  Created on: Oct 3, 2024
 *      Author: isaac
 */


#include "wah_wah.h"
void initialize_LOWFREQOSC (LOWFREQOSC *osc, float amplitude, float minAmp,
		float maxAmp, float osc_freq, float minFreq, float maxFreq, float phaseAngle, float sampleFreq);
void intialize_SVFILTER (SVFILTER *svf, float centerFreq, float damping, float sampleRate);
void initialize_WAHWAH(WAHWAH * ww, float wahFreq, float minCutoffFreq, float maxCutoffFreq, float damp, float wet_dry, float sampleRate) {
	ww->maxCutoffFreq = maxCutoffFreq;
	ww->minCutoffFreq = minCutoffFreq;
	initialize_LOWFREQOSC (&(ww->lfo), maxCutoffFreq - minCutoffFreq, 0.0f,
			5000.0f, wahFreq / 2.0f, 0.01f, -1.0f, 0, sampleRate);
	intialize_SVFILTER (&(ww->svf), minCutoffFreq, damp, sampleRate);
	initialize_MIXER (&(ww->mixer), wet_dry);
}

float apply_WAHWAH (WAHWAH * ww, float input) {
	update_LOWFREQOSC(&(ww->lfo));
	// first step should be to set the cutoff frequency for the filter
	set_frequency_damping_SVFILER(&(ww->svf), ww->minCutoffFreq + fabs(ww->lfo.out), -1);
	apply_SVFILTER (&(ww->svf), input);
	applyWetDry_MIXER (&(ww->mixer), ww->svf.ybOut, input);
 	ww->out =  ww->mixer.out;
	return ww->out;
}

void setMInMAXCutoffFrequency (WAHWAH *ww, float minCutoffFreq, float maxCutoffFreq) {
	minCutoffFreq = (minCutoffFreq < 0.0f) ? ww->minCutoffFreq : minCutoffFreq;
	maxCutoffFreq = (maxCutoffFreq < 0.0f) ? ww->maxCutoffFreq : maxCutoffFreq;
	ww->minCutoffFreq = minCutoffFreq;
	ww->maxCutoffFreq = maxCutoffFreq;
	set_frequency_damping_SVFILER(&(ww->svf), ww->minCutoffFreq, -1);
	setAmplitude_LOWFREQOSC(&(ww->lfo), maxCutoffFreq - minCutoffFreq);
}

void setWahFreq_Damping_WAHWAH(WAHWAH * ww, float wahFreq, float damp) {
	// when we take the absolute value that doubles the lfo frequency
	setFreq_LOWFREQOSC(&(ww->lfo), wahFreq / 2.0f);
	set_frequency_damping_SVFILER(&(ww->svf), -1.0f, damp);
}

