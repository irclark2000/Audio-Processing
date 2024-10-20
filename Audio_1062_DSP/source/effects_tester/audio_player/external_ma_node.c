/*
 * external_node.c
 *
 *  Created on: Oct 18, 2024
 *      Author: isaac
 */

/*
 * external_node.c
 *
 *  Created on: Oct 18, 2024
 *      Author: isaac
 */

#if AUDIO_EFFECTS_TESTER
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

#endif
