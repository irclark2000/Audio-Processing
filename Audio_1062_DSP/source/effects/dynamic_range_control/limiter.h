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
	float threshold_db;  //db
	float makeup_gain; //db
	float knee;
	float absInput;
	float attack_time;
	float release_time;
	float alphaA, alphaR;
	int makeup_property_mode;
	float compress_out;
} LIMITER;

void initialize_LIMITER (LIMITER *limiter, float sample_rate);
float update_LIMITER (LIMITER *limiter, float input);
void gui_setAttackRelease_LIMITER(LIMITER *limiter);
void setRelease_LIMITER(LIMITER *limiter, float release_time);
void setAttack_LIMITER(LIMITER *limiter, float attack_time);
void setTreshold_LIMITER(LIMITER * limiter, float threshold_db);
void setKneeWidth_LIMITER(LIMITER * limiter, float knee_width);


#endif /* EFFECTS_DYNAMIC_RANGE_CONTROL_LIMITER_H_ */
