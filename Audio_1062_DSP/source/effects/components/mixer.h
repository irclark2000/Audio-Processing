/*
 * mixer.h
 *
 *  Created on: Sep 23, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_COMPONENTS_MIXER_H_
#define EFFECTS_COMPONENTS_MIXER_H_

typedef struct {
	float wet_dry;
} MIXER;

void initialize_MIXER (MIXER * mixer, float wet_dry);
float applyWetDry_MIXER (MIXER * mixer, float input_wet, float input_dry);
void setWetDry_MIXER (MIXER *mixer, float wet_dry);
#endif /* EFFECTS_COMPONENTS_MIXER_H_ */
