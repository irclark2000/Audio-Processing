/*
 * external_ma_node.h
 *
 *  Created on: Oct 18, 2024
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

#ifndef EFFECTS_TESTER_AUDIO_PLAYER_EXTERNAL_MA_NODE_H_
#define EFFECTS_TESTER_AUDIO_PLAYER_EXTERNAL_MA_NODE_H_

#include "miniaudio.h"
#include "apply_effect.h"
//#include "effect_component.h"
#include "audio_component.h"
#include <stdint.h>

typedef struct {
	ma_uint32 channels;
	ma_uint32 sampleRate;
	//EFFECT_COMPONENT *component;
	AUDIO_COMPONENT *aChannels;
}ma_effects_config;

typedef struct {
	ma_node_config nodeConfig;
	ma_effects_config effect;
} ma_effects_node_config;

typedef struct
{
	ma_effects_config config;
} ma_effects;

typedef struct {
	ma_node_base baseNode;
	ma_effects effects;
} ma_effects_node;

MA_API ma_effects_config ma_effects_config_init(ma_uint32 channels, ma_uint32 sampleRate, AUDIO_COMPONENT *aChannels);
MA_API ma_result ma_effects_init(const ma_effects_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_effects *pEffect);
MA_API void ma_effects_uninit(ma_effects* pEffect, const ma_allocation_callbacks* pAllocationCallbacks);
MA_API ma_effects_config ma_effects_config_init(ma_uint32 channels, ma_uint32 sampleRate, AUDIO_COMPONENT *aChannels);
MA_API ma_effects_node_config ma_effects_node_config_init(ma_uint32 channels, ma_uint32 sampleRate, AUDIO_COMPONENT *aChannels);
MA_API ma_result ma_effects_node_init(ma_node_graph* pNodeGraph, const ma_effects_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_effects_node* pEffectsNode);
MA_API void ma_effects_node_uninit(ma_effects_node* pEffectsNode, const ma_allocation_callbacks* pAllocationCallbacks);
MA_API ma_result ma_effects_process_pcm_frames(ma_effects* pEffects, void* pFramesOut, const void* pFramesIn, ma_uint32 frameCount);
MA_API void ma_effects_set_parameter(ma_effects* pEffect, AUDIO_COMPONENT *aChannels, float value);
MA_API float ma_effects_get_parameter(const ma_effects* pEffect, AUDIO_COMPONENT *aChannels);


#endif /* EFFECTS_TESTER_AUDIO_PLAYER_EXTERNAL_MA_NODE_H_ */
