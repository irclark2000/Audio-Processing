/*
 * all_pass1.h
 *
 *  Created on: May 13, 2022
 *      Author: isaac
 */

#ifndef EFFECTS_FILTERS_ALL_PASS1_H_
#define EFFECTS_FILTERS_ALL_PASS1_H_

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
void uninitialize_AllpassFilter1(ALLPASS1 *pass);
#endif /* EFFECTS_FILTERS_ALL_PASS1_H_ */
