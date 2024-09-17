/*
 * high_pass.c
 *
 *  Created on: May 11, 2022
 *      Author: isaac
 */

#include "high_pass.h"

void initHighPass(HIGHPASS *hp, float a0, float a1, float a2, float b1, float b2) {
	hp->a0 = a0;
	hp->a1 = a1;
	hp->a2 = a2;
	hp->b1 = b1;
	hp->b2 = b2;
	hp->in_z1 = 0.0f;
	hp->in_z2 = 0.0f;
	hp->out_z1 = 0.0f;
	hp->out_z2 = 0.0f;
}

float applyHighPass(HIGHPASS *hp, float input) {
	hp->out = hp->a0 * input
			+ hp->a1 * hp->in_z1
			+ hp->a2 * hp->in_z2
			- hp->b1 * hp->out_z1
			- hp->b2 * hp->out_z2;
	hp->in_z2 = hp->in_z1;
	hp->in_z1 = input;
	hp->out_z2 = hp->out_z1;
	hp->out_z1 = hp->out;

	return hp->out;
}

