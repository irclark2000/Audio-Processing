/*
 * high_pass.h
 *
 *  Created on: May 11, 2022
 *      Author: isaac
 */

#ifndef HIGH_PASS_H_
#define HIGH_PASS_H_

typedef struct {
	float a0, a1, a2;
	float b1, b2;
	float in_z1;
	float in_z2;
	float out_z1;
	float out_z2;
	float out;
}HIGHPASS;

void initHighPass(HIGHPASS *hp, float a0, float a1, float a2, float b1, float b2);
float applyHighPass(HIGHPASS *hp, float input);

#endif /* HIGH_PASS_H_ */
