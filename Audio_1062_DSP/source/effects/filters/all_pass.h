/*
 * all_pass.h
 *
 *  Created on: Apr 9, 2022
 *      Author: isaac
 */

#ifndef EFFECTS_FILTERS_ALL_PASS_H_
#define EFFECTS_FILTERS_ALL_PASS_H_

typedef struct {
	float	feedback;
	float	*buf;
	int		bufSize;
	int		bufIndx;
	float 	out;
	float 	bufOut;
	int 	i;

} ALLPASS;

void initAllpassFilter(ALLPASS * pass, float *buffer, int size, float feedback);
float applyAllpassFilter(ALLPASS *pass, float input);

#endif /* EFFECTS_FILTERS_ALL_PASS_H_ */
