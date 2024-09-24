/*
 * mixer.c
 *
 *  Created on: Sep 23, 2024
 *      Author: isaac
 */

#include "mixer.h"
void initialize_MIXER (MIXER * mixer, float wet_dry) {
	setWetDry_MIXER(mixer, wet_dry);
}
float apply_MIXER (MIXER * mixer, float input_wet, float input_dry){
	return input_dry * (1.0f - mixer->wet_dry) + input_wet * mixer->wet_dry;
}
void setWetDry_MIXER (MIXER *mixer, float wet_dry){
	mixer->wet_dry = wet_dry < 0.0f ? 0.0f : (wet_dry > 1.0f ? 1.0f : wet_dry);
}
