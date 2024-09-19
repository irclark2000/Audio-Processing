/*
 * limiter.h
 *
 *  Created on: Sep 19, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_DYNAMIC_RANGE_CONTROL_LIMITER_H_
#define EFFECTS_DYNAMIC_RANGE_CONTROL_LIMITER_H_


#include <stdint.h>
#include <math.h>

typedef struct {
	float sample_time;
	float gs;  //db
	float threshold;  //db
	float makeup_gain; //db
	float knee;
	float attack_time;
	float release_time;
	float alphaA, alphaR;
	uint8_t hard_knee;
	uint8_t makeup_property_mode;
	float compress_out;
} LIMITER;

void initialize_limiter (LIMITER *limiter, float sample_rate);
float update_limiter (LIMITER *limiter, float input);
void limiter_setRelease(LIMITER *limiter, float release_time);
void limiter_setAttack(LIMITER *limiter, float attack_time);


#endif /* EFFECTS_DYNAMIC_RANGE_CONTROL_LIMITER_H_ */
