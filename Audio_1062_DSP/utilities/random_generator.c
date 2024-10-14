/*
 * random_generator.c
 *
 *  Created on: Sep 11, 2024
 *      Author: isaac
 */

#include "random_generator.h"

static uint32_t seed;

uint32_t rand31_next (void);

void intitialize_random_number_generator(void) {
    trng_config_t trngConfig;
    status_t status;

    TRNG_GetDefaultConfig(&trngConfig);

    /* Initialize TRNG */
    status = TRNG_Init(TRNG0, &trngConfig);
	uint32_t data;
    status = TRNG_GetRandomData(TRNG0, &data, sizeof(data));
    if (status == kStatus_Success) {
    	seed = data;
    }
    else {
        seed = 0;
    }
}
const static float INT32_TO_FLOAT = 1.0f / 1073741824.0f;

float get_random_float(void) {
	return ((int32_t)rand31_next()) * INT32_TO_FLOAT;
}

uint32_t rand31_next () {
	uint32_t lo, hi;
	lo = 16807 * (seed & 0xFFFF);
	hi = 16807 * (seed >> 16);
	lo  += (hi & 0x7FFF) << 16;
	lo += hi >> 15;
	if (lo > 0x7FFFFFFF) lo -= 0x7FFFFFFF;
	seed = (long) lo;
	return seed;
}
