/*
 * all_pass.c
 *
 *  Created on: Apr 9, 2022
 *      Author: isaac
 */

#include <filters/all_pass.h>
#include "denormals.h"

static void mute (ALLPASS *pass);

void initAllpassFilter(ALLPASS * pass, float *buffer, int size, float feedback) {
	pass->buf = buffer;
	pass->bufSize = size;
	pass->feedback = feedback;
	pass->bufIndx = 0;
	mute(pass);
}

float applyAllpassFilter(ALLPASS *pass, float input) {

		pass->bufOut = pass->buf[pass->bufIndx];
		undenormalise(pass->bufOut);

		pass->out = -input + pass->bufOut;
		pass->buf[pass->bufIndx] = input + (pass->bufOut*pass->feedback);

		if(++(pass->bufIndx)>=pass->bufSize) pass->bufIndx = 0;

		return pass->out;
}

static void mute (ALLPASS *pass) {
	for(pass->i = 0; pass->i < pass->bufSize; pass->i++) {
		pass->buf[pass->i] = 0.0f;
	}
}
