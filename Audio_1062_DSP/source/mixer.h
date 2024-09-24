/*
 * mixer.h
 *
 *  Created on: Sep 23, 2024
 *      Author: isaac
 */

#ifndef MIXER_H_
#define MIXER_H_

typedef struct {
	float wet_dry;
} MIXER;

void initialize_MIXER (MIXER * mixer, float wet_dry);
float apply_MIXER (MIXER * mixer, float input_wet, float input_dry);
void setWetDry_MIXER (MIXER *mixer, float wet_dry);
#endif /* MIXER_H_ */
