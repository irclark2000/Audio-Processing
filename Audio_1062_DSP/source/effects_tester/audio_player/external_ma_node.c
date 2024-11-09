/*
 * external_node.c
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


#if AUDIO_EFFECTS_TESTER
#define MINIAUDIO_IMPLEMENTATION

#include "external_ma_node.h"

MA_API ma_result ma_effects_init(const ma_effects_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_effects *pEffect) {
    if (pEffect == NULL) {
        return MA_INVALID_ARGS;
    }

    MA_ZERO_OBJECT(pEffect);

    if (pConfig == NULL) {
        return MA_INVALID_ARGS;
    }

    pEffect->config             = *pConfig;
    return MA_SUCCESS;
}

MA_API void ma_effects_uninit(ma_effects* pEffects, const ma_allocation_callbacks* pAllocationCallbacks) {
}
MA_API void ma_effects_set_parameter(ma_effects* pEffect, EFFECT_COMPONENT *component, float value) {

}
MA_API float ma_effects_get_parameter(const ma_effects* pEffect, EFFECT_COMPONENT *component) {
	return 0.0f;
}
MA_API ma_effects_node_config ma_effects_node_config_init(ma_uint32 channels, ma_uint32 sampleRate, EFFECT_COMPONENT *component)
{
    ma_effects_node_config config;

    config.nodeConfig = ma_node_config_init();
    config.effect = ma_effects_config_init(channels, sampleRate, component);

    return config;
}
MA_API ma_effects_config ma_effects_config_init(ma_uint32 channels, ma_uint32 sampleRate, EFFECT_COMPONENT *component)
{
    ma_effects_config config;

    MA_ZERO_OBJECT(&config);
    config.channels      = channels;
    config.sampleRate    = sampleRate;
    config.component = component;
   /*
    config.delayStart    = (decay == 0) ? MA_TRUE : MA_FALSE;
    config.wet           = 1;
    config.dry           = 1;
    config.decay         = decay;
 */
    return config;
}


static void ma_effects_node_process_pcm_frames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut)
{
    ma_effects_node* pEffectsNode = (ma_effects_node*)pNode;

    (void)pFrameCountIn;

    ma_effects_process_pcm_frames(&pEffectsNode->effects, ppFramesOut[0], ppFramesIn[0], *pFrameCountOut);
}

static ma_node_vtable g_ma_effects_node_vtable =
{
    ma_effects_node_process_pcm_frames,
    NULL,
    1,  /* 1 input channels. */
    1,  /* 1 output channel. */
    MA_NODE_FLAG_CONTINUOUS_PROCESSING  /* Delay requires continuous processing to ensure the tail get's processed. */
};


MA_API ma_result ma_effects_process_pcm_frames(ma_effects* pEffects, void* pFramesOut, const void* pFramesIn, ma_uint32 frameCount)
{
    ma_uint32 iFrame;
    ma_uint32 iChannel;
    float* pFramesOutF32 = (float*)pFramesOut;
    const float* pFramesInF32 = (const float*)pFramesIn;

    if (pEffects == NULL || pFramesOut == NULL || pFramesIn == NULL) {
        return MA_INVALID_ARGS;
    }
    EFFECT_COMPONENT *component = pEffects->config.component;

    for (iFrame = 0; iFrame < frameCount; iFrame += 1) {
	    for (iChannel = 0; iChannel < pEffects->config.channels; iChannel += 1) {

		    //float out = component->apply(component->effect, pFramesInF32[iChannel]);
		    // Just copying for now
		    pFramesOutF32[iChannel] = pFramesInF32[iChannel];
		    //pFramesOutF32[iChannel] = out;
	    }
	    pFramesOutF32 += pEffects->config.channels;
	    pFramesInF32  += pEffects->config.channels;
    }
    return MA_SUCCESS;
}

MA_API ma_result ma_effects_node_init(ma_node_graph* pNodeGraph, const ma_effects_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_effects_node* pEffectsNode)
{
	ma_result result;
	ma_node_config baseConfig;

	if (pEffectsNode == NULL) {
		return MA_INVALID_ARGS;
	}

	MA_ZERO_OBJECT(pEffectsNode);

	result = ma_effects_init(&pConfig->effect, pAllocationCallbacks, &pEffectsNode->effects);
	if (result != MA_SUCCESS) {
		return result;
	}

	baseConfig = pConfig->nodeConfig;
	baseConfig.vtable          = &g_ma_effects_node_vtable;
	baseConfig.pInputChannels  = &pConfig->effect.channels;
	baseConfig.pOutputChannels = &pConfig->effect.channels;

	result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks, &pEffectsNode->baseNode);
	if (result != MA_SUCCESS) {
		ma_effects_uninit(&pEffectsNode->effects, pAllocationCallbacks);
		return result;
	}

	return result;
}

MA_API void ma_effects_node_uninit(ma_effects_node* pEffectsNode, const ma_allocation_callbacks* pAllocationCallbacks)
{
	if (pEffectsNode == NULL) {
		return;
	}

	/* The base node is always uninitialized first. */
	ma_node_uninit(pEffectsNode, pAllocationCallbacks);
	ma_effects_uninit(&pEffectsNode->effects, pAllocationCallbacks);
}

#endif
