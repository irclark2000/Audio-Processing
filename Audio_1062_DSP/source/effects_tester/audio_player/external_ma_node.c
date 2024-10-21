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

MA_API ma_result ma_effects_init(const ma_effects_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, EFFECT_PARAMS* parameter);
MA_API void ma_effects_uninit(ma_effects* pEffects, const ma_allocation_callbacks* pAllocationCallbacks) {
}
MA_API void ma_effects_set_parameter(ma_effects* pEffect, EFFECT_PARAMS *parameter, float value, uint8_t pIndex) {
	parameter->floatParameter[pIndex] = value;
}
MA_API float ma_effects_get_parameter(const ma_effects* pEffect, EFFECT_PARAMS *parameter, uint8_t pIndex) {
	return parameter->floatParameter[pIndex];
}
MA_API ma_effects_node_config ma_effects_node_config_init(ma_uint32 channels, ma_uint32 sampleRate, ma_uint32 delayInFrames, EFFECT_PARAMS *parameters)
{
    ma_effects_node_config config;

    config.nodeConfig = ma_node_config_init();
    config.effects = ma_effects_config_init(channels, sampleRate, delayInFrames, parameters);

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

MA_API ma_effects_config ma_effects_config_init(ma_uint32 channels, ma_uint32 sampleRate, ma_uint32 delayInFrames, EFFECT_PARAMS *parameters)
{
    ma_effects_config config;

    MA_ZERO_OBJECT(&config);
    config.channels      = channels;
    config.sampleRate    = sampleRate;
    config.delayInFrames = delayInFrames;
   /*
    config.delayStart    = (decay == 0) ? MA_TRUE : MA_FALSE;
    config.wet           = 1;
    config.dry           = 1;
    config.decay         = decay;
 */
    return config;
}

MA_API ma_result ma_effects_process_pcm_frames(ma_effects* pEffects, void* pFramesOut, const void* pFramesIn, ma_uint32 frameCount)
{
    ma_uint32 iFrame;
    ma_uint32 iChannel;
    float* pFramesOutF32 = (float*)pFramesOut;
    const float* pFramesInF32 = (const float*)pFramesIn;

    if (pEffects == NULL || pFramesOut == NULL || pFramesIn == NULL) {
        return MA_INVALID_ARGS;
    }

    for (iFrame = 0; iFrame < frameCount; iFrame += 1) {
        for (iChannel = 0; iChannel < pEffects->config.channels; iChannel += 1) {
            ma_uint32 iBuffer = (pEffects->cursor * pEffects->config.channels) + iChannel;

            if (0) {
                /* Delayed start. */
#if 0
                /* Read */
                pFramesOutF32[iChannel] = pEffects->pBuffer[iBuffer] * pEffects->config.wet;

                /* Feedback */
                pEffects->pBuffer[iBuffer] = (pEffects->pBuffer[iBuffer] * pDelay->config.decay) + (pFramesInF32[iChannel] * pDelay->config.dry);
#endif
            } else {
                /* Immediate start */
#if 0
                /* Feedback */
                pDelay->pBuffer[iBuffer] = (pEffects->pBuffer[iBuffer] * pDelay->config.decay) + (pFramesInF32[iChannel] * pDelay->config.dry);

                /* Read */
                // Just copying for now
#endif
                pFramesOutF32[iChannel] = pEffects->pBuffer[iBuffer];
            }
        }

        pEffects->cursor = (pEffects->cursor + 1) % pEffects->bufferSizeInFrames;

        pFramesOutF32 += pEffects->config.channels;
        pFramesInF32  += pEffects->config.channels;
    }

    return MA_SUCCESS;
}


#endif
