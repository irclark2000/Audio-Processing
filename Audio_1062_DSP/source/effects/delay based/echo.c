/*
 * echo.c
 *
 *  Created on: Sep 21, 2024
 *      Author: isaac
 */


#include "echo.h"


void intialize_ECHO (ECHO *echo, float *buf, uint32_t buf_size, float delayMSec, float feedback_level, float sampleRate) {
	initialize_variable_delay (&(echo->vDelay), buf, buf_size, sampleRate);
	setDelayMSec_ECHO (echo, delayMSec);
	setFeedback_level_ECHO (echo, feedback_level);
}
void setDelayMSec_ECHO (ECHO *echo, float delayMSec) {
	setDelay_VARDELAY(&(echo->vDelay), delayMSec / 1000.0f);
}
void setFeedback_level_ECHO (ECHO *echo, float level) {
	echo->feedBack_level = (level < 0.0f) ? 0 : ((level > 1.0f) ? 1.0 : level);
}

float getMaxDelayMS_ECHO (ECHO *echo) {
	return echo->vDelay.max_delay * 1000.0f;
}
float update_Echo (ECHO * echo, float input) {
	// get sample should be called before adding new sample to buffer
	// add feedback (with gain) to the input
	float output = getDelayedSample_VARDELAY(&(echo->vDelay), input, echo->feedBack_level);
	return output;
}