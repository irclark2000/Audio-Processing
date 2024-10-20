/*
 * external_ma_node.h
 *
 *  Created on: Oct 18, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_TESTER_AUDIO_PLAYER_EXTERNAL_MA_NODE_H_
#define EFFECTS_TESTER_AUDIO_PLAYER_EXTERNAL_MA_NODE_H_

#include "miniaudio.h"
#include "apply_effect.h"
#include <stdint.h>

typedef struct {
    ma_uint32 channels;
    ma_uint32 sampleRate;
    ma_uint32 delayInFrames;
        EFFECT_PARAMS parameters;
}ma_effects_config;

MA_API ma_effects_config ma_effects_config_init(ma_uint32 channels, ma_uint32 sampleRate, ma_uint32 delayInFrames,
                EFFECT_PARAMS *eParamters);
typedef struct
{
    ma_effects_config config;
    ma_uint32 cursor;               /* Feedback is written to this cursor. Always equal or in front of the read cursor. */
    ma_uint32 bufferSizeInFrames;
    float* pBuffer;
} ma_effects;

MA_API ma_result ma_effects_init(const ma_effects_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, EFFECT_PARAMS* parameters);
MA_API void ma_effects_uninit(ma_effects* pEffect, const ma_allocation_callbacks* pAllocationCallbacks);
MA_API void ma_effects_set_parameter(ma_effects* pEffect, EFFECT_PARAMS *parameter, float value, uint8_t pIndex);
MA_API float ma_effects_get_parameter(const ma_effects* pEffect, EFFECT_PARAMS *parameter, uint8_t pIndex);

#endif /* EFFECTS_TESTER_AUDIO_PLAYER_EXTERNAL_MA_NODE_H_ */
                                                                                                                                                                            31,0-1        Top

