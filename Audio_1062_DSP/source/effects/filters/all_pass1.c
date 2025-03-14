/*
 * all_pass1.c
 *
 *  Created on: May 13, 2022
 *      Author: isaac
 */

#include <filters/all_pass1.h>
#include <hardware_specific/denormals.h>
#if AUDIO_EFFECTS_TESTER
#include <stdlib.h>
#endif

static void mute (ALLPASS1 *pass);

void initAllpassFilter1(ALLPASS1 * pass, float *buffer, int size, float gain) {
#if AUDIO_EFFECTS_TESTER
	if (buffer == 0) {
		buffer = (float *) malloc(size * sizeof(float));
	}
#endif
	pass->buf = buffer;
	pass->bufSize = size;
	pass->gain = gain;
	pass->bufIndx = 0;
	mute(pass);
}

float applyAllpassFilter1(ALLPASS1 *pass, float input) {

		pass->bufOut = pass->buf[pass->bufIndx];
		undenormalise(pass->bufOut);
#if 0

		pass->bufOut = -pass->gain *input + pass->bufOut;
		pass->next = pass->bufOut * pass->gain + input;
		pass->buf[pass->bufIndx] = pass->next;
		if(++(pass->bufIndx)>=pass->bufSize) pass->bufIndx = 0;
		return pass->bufOut;
#else
		pass->next = pass->bufOut * pass->gain + input;
		pass->out = pass->bufOut - pass->gain * pass->next;
		pass->buf[pass->bufIndx] = pass->next;
		if(++(pass->bufIndx)>=pass->bufSize) pass->bufIndx = 0;
		return pass->out;
#endif
}

static void mute (ALLPASS1 *pass) {
	for(pass->i = 0; pass->i < pass->bufSize; pass->i++) {
		pass->buf[pass->i] = 0.0f;
	}
}

void uninitialize_AllpassFilter1(ALLPASS1 *pass1) {
#if AUDIO_EFFECTS_TESTER
	free(pass1->buf);
#endif
}

