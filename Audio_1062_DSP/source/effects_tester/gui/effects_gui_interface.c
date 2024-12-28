/*
 * effects_gui_interface.c
 *
 *  Created on: Oct 24, 2024
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
#include "effects_gui_interface.h"
#include "effects/tremolo.h"
#include "effects/overdrive.h"
#include "effects/delay_based/echo.h"
#include "effects/dynamic_range_control/noise_gate.h"
#include "effects/dynamic_range_control/compressor.h"
#include "effects/dynamic_range_control/expander.h"
#include "effects/dynamic_range_control/limiter.h"
#include "effects/reverbs/freeverb.h"
#include "effects/reverbs/schroeder_verb.h"
#include "effects/reverbs/moorer_reverb.h"
#include "effects/variable_filter_effects/equalizer.h"
#include "fast_math/fast_math.h"
#include <string.h>
#if AUDIO_EFFECTS_TESTER
#include <stdlib.h>
#endif

EFFECT_ITEM delay_based_effect[] = {
		{"Echo", Echo}, {"Flanger", Flanger}, {"Vibrato", Vibrato},
		{"Chorus", Chorus}
};
EFFECT_ITEM reverb_effect[] = {
		{"Freeverb", Freeverb}, {"Schroeder Reverb", Schroeder}, {"Moorer Reverb", Moorer}
};
EFFECT_ITEM dynamic_range_control_effect[] = {
		{"Compressor", Compressor}, {"Expander", Expander},
		{"Limiter", Limiter}, {"Noise Gate", Noisegate}
};
EFFECT_ITEM misc_effect[] = {
		{"Phaser", Phaser}, {"Overdrive", Overdrive},
		{"Pitch Shifter", PitchShift}, {"Tremolo", Tremolo},{"Wah Wah", WahWah},
		{"Auto Wah", AutoWah}
};

EFFECT_ITEM effects_list[] = {
	{"Auto Wah", AutoWah}, {"Chorus", Chorus}, {"Echo", Echo}, {"Flanger", Flanger}, {"Vibrato", Vibrato},
	{"Freeverb", Freeverb}, {"Schroeder Reverb", Schroeder}, {"Moorer Reverb", Moorer},
	{"Equalizer", Equalizer},
	{"Tremolo", Tremolo}, {"Noise Gate", Noisegate}, {"Overdrive", Overdrive},
	{"Compressor", Compressor}, {"Limiter", Limiter}, {"Expander", Expander}, {"", None}
};


void gui_initialize(EFFECT_COMPONENT *component, uint32_t size, float sampleRate) {
	switch(component->type) {
		case AutoWah:
			{
				AUTOWAH *aw = component->effect;
				aw->sampleRate = sampleRate;
				for(int i=0; i < component->childrenCount; ++i) {
					gui_initialize(component->childComponents[i], 0, sampleRate);
				}
			}
			break;
		case Chorus:
			{
				CHORUS *chorus = component->effect;
				uint8_t chorus_count = chorus->chorus_count;
				chorus->sampleRate = sampleRate;
				chorus->inv_count = 1.0f / chorus_count;
				for(int i=0; i < component->childrenCount; ++i) {
					gui_initialize(component->childComponents[i], 0, sampleRate);
				}
			}
			break;
		case ChorusElement:
			{
				CHORUSELEMENT *cElement = component->effect;
				float value = cElement->baseDelayMSec;
				for(int i=0; i < component->childrenCount; ++i) {
					gui_initialize(component->childComponents[i], 0, sampleRate);
				}
			}
			break;
		case Compressor:
				{
					COMPRESSOR *dr = component->effect;
					initialize_COMPRESSOR(dr, sampleRate);
					dr->hard_knee = (dr->gui_not_hard_knee == 0) ? 1 : 0;
					dr->makeup_property_mode = (dr->gui_not_hard_property == 0) ? 1 : 0;
				}
				break;
		case Expander:
				{
					EXPANDER *dr = component->effect;
					initialize_EXPANDER(dr, sampleRate);
					dr->hard_knee = (dr->gui_not_hard_knee == 0) ? 1 : 0;
				}
				break;
		case Limiter:
				{
					LIMITER *dr = component->effect;
					initialize_LIMITER(dr, sampleRate);
					dr->makeup_property_mode = (dr->gui_not_hard_property == 0) ? 1 : 0;
				}
				break;
		case Echo:
			{
				//ECHO *echo = component->effect;
				for(int i=0; i < component->childrenCount; ++i) {
					gui_initialize(component->childComponents[i], 0, sampleRate);
				}
			}
			break;
		case Flanger:
			{
				//FLANGER *flanger = component->effect;
				for(int i=0; i < component->childrenCount; ++i) {
					gui_initialize(component->childComponents[i], 0, sampleRate);
				}
			}
			break;
		case Equalizer:
		{
			for(int i=0; i < component->childrenCount; ++i) {
				gui_initialize(component->childComponents[i], 0, sampleRate);
			}
		}
		break;
		case EqualizingFilter:
		{
			EQFILTER *eqf = component->effect;
			eqf->sampleTime = 1.0/sampleRate;
			eqf->gain = fastPow10(eqf->gui_gainDB/20.0f);
			setCenterFrequency_EQFILTER(eqf, eqf->gui_freq, eqf->gui_freq/eqf->Q);
		}
		break;
		case EnvelopeFollower:
			{
				ENVELOPE_FOLLOWER *ef = component->effect;
				ef->sampleRate = sampleRate;
				gui_setAttackRelease_ENVELOPE_FOLLOWER(ef);
			}
			break;
		case Freeverb:
			{
				FREEVERB *fv = component->effect;
				initFreeverb(fv, sampleRate);
			}
			break;
		case Lfo:
			{
				LOWFREQOSC *osc = component->effect;
				osc->sampleTime = 1.0f / sampleRate;
				osc->sampleRate = sampleRate;
				osc->direction = 1.0f;
				osc->counter = 0.0f;
				osc->frequency_limits.minimum = 0.01;
				osc->frequency_limits.maximum = 0.25 * sampleRate;
				osc->amplitude_limits.minimum = 0.0;
				osc->amplitude_limits.maximum = 1E9; 
				gui_setFrequency(osc);
			}
			break;
		case Moorer:
		{
			MOORER_REVERB *mr = component->effect;
			gui_initialize_MOORER_REVERB(mr, sampleRate);
		}
		case Noisegate:
			{
				NOISEGATE *ng = component->effect;
				ng->smooth_gain = 0.0f;
				ng->static_gain = 1.0f;
				ng->sampleTimeMs = 1000.0f/sampleRate;
				ng->attack_time_accumulator = 0.0f;
				ng->release_time_accumulator = 0.0f;
			}
			break;
		case Overdrive:
			{
				OVERDRIVE *od = component->effect;
				initialize_OVERDRIVE(od, sampleRate, od->gui_HPFFreq,
						od->preGain, od->gui_LPFFreq, od->lpfOutDamp);
			}
			break;
		case Schroeder:
			{
				SCHROEDERVERB *fv = component->effect;
				initSchroederVerb(fv, sampleRate);
			}
			break;
		case Tremolo:
		{
			TREMOLO *trem = component->effect;
			trem->sampleRate = sampleRate;
			for(int i=0; i < component->childrenCount; ++i) {
				gui_initialize(component->childComponents[i], 0, sampleRate);
			}
		}
		break;
		case VariableDelay:
			{
				VARDELAY *vDelay = component->effect;
				vDelay->sampleTime = 1.0f / sampleRate;
				vDelay->sampleRate = sampleRate;
				uint32_t max_size = sampleRate * vDelay->max_delay;
				vDelay->delayInSamples = sampleRate * vDelay->gui_delayMSec * 0.001f; 
				vDelay->cBufPtr = &(vDelay->cBuf);
				vDelay->size = max_size + 1;
				cb_initialize(vDelay->cBufPtr, 0, max_size + 1);	
			}
			break;
		case VariableBandpass:
			{
				VARBANDPASS *vbf = component->effect;
				initialize_SECONDALLPASSFILTER(&(vbf->apf), vbf->gui_Freq, vbf->gui_Q, sampleRate);
			}
			break;
		case Vibrato:
			{
				VIBRATO *vb = component->effect;
				for(int i=0; i < component->childrenCount; ++i) {
					gui_initialize(component->childComponents[i], 0, sampleRate);
				}
			}
			break;
		case Volume:
		case Mixer:
			break;   
		default:
			break;
	}
}
