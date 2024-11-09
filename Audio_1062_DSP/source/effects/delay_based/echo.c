/*
 * echo.c
 *
 *  Created on: Sep 21, 2024
 *      Author: isaac
 */


#include <delay_based/echo.h>


void intialize_ECHO (ECHO *echo, float *buf, uint32_t buf_size, float delayMSec,
		float feedback_level, float feedback_gain, float wet_dry, float sampleRate) {
#if AUDIO_EFFECTS_TESTER
	initialize_variable_delay (&(echo->vDelay), 0, buf_size, sampleRate);
#else
	initialize_variable_delay (&(echo->vDelay), buf, buf_size, sampleRate);
#endif
	setDelayMSec_ECHO (echo, delayMSec);
	setFeedback_level_ECHO (echo, feedback_level);
	setFeedback_gain_ECHO (echo, feedback_gain);
	initialize_MIXER (&(echo->mixer), wet_dry);
}
void setDelayMSec_ECHO (ECHO *echo, float delayMSec) {
	setDelay_VARDELAY(&(echo->vDelay), delayMSec / 1000.0f);
}
void setFeedback_level_ECHO (ECHO *echo, float level) {
	echo->feedback_level = (level < 0.0f) ? 0 : ((level > 1.0f) ? 1.0 : level);
}
void setFeedback_gain_ECHO (ECHO *echo, float gain) {
	echo->feedback_gain = gain;
}

float getMaxDelayMS_ECHO (ECHO *echo) {
	return echo->vDelay.max_delay * 1000.0f;
}
static float delayed;
static float wet;

float update_Echo (void * vEcho, float input) {
	ECHO * echo = (ECHO *)vEcho;
	// get sample should be called before adding new sample to buffer
	// add feedback (with gain) to the input
	delayed = getDelayedSample_VARDELAY(&(echo->vDelay), input, echo->feedback_level);
	wet = echo->feedback_gain * delayed;
	echo->out = applyWetDry_MIXER (&(echo->mixer), wet, input);

	return echo->out;
}

#if AUDIO_EFFECTS_TESTER
void uninitialize_ECHO(ECHO *echo) {
	uninitialize_VARDELAY(&(echo->vDelay));
}
#endif
