/*
 * echo.h
 *
 *  Created on: Sep 21, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_DELAY_BASED_ECHO_H_
#define EFFECTS_DELAY_BASED_ECHO_H_

#include "variable_delay.h"
#include "mixer.h"

typedef struct {
	VARDELAY vDelay;
	float feedBack_level;
	float feedback_gain;
	MIXER mixer;
} ECHO;

void intialize_ECHO (ECHO *echo, float *buf, uint32_t buf_size, float delayMSec,
		float feedback_level, float feedback_gain, float wet_dry, float sampleRate);
void setDelayMSec_ECHO (ECHO *echo, float delayMSec);
void setFeedback_level_ECHO (ECHO *echo, float level);
void setFeedback_gain_ECHO (ECHO *echo, float gain);
void setWetDry_ECHO (ECHO *echo, float wet_dry);
float update_Echo (ECHO * echo, float input);
float getMaxDelayMS_ECHO (ECHO *echo);
#endif /* EFFECTS_DELAY_BASED_ECHO_H_ */
