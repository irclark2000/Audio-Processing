/*
 * all_pass1.h
 *
 *  Created on: May 13, 2022
 *      Author: isaac
 */

#ifndef EFFECTS_ALL_PASS1_H_
#define EFFECTS_ALL_PASS1_H_

typedef struct {
	float	gain;
	float	*buf;
	int		bufSize;
	int		bufIndx;
	float 	out;
	float 	bufOut;
	float 	next;
	int 	i;

} ALLPASS1;

void initAllpassFilter1(ALLPASS1 * pass, float *buffer, int size, float gain);
float applyAllpassFilter1(ALLPASS1 *pass, float input);

#endif /* EFFECTS_ALL_PASS1_H_ */
