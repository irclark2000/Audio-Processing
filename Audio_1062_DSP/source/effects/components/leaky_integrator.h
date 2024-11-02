/*
 * leaky_integrator.h
 *
 *  Created on: Nov 1, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_COMPONENTS_LEAKY_INTEGRATOR_H_
#define EFFECTS_COMPONENTS_LEAKY_INTEGRATOR_H_
#include <stdint.h>

typedef struct {
	uint8_t N;
	float liOut;
	float Yn_1;
	float inv_N;
}LEAKY_INTEGRATOR;

void intialize_LEAKY_INTEGRATOR (LEAKY_INTEGRATOR *li, uint8_t N);
float apply_LEAKY_INTEGRATOR (LEAKY_INTEGRATOR *li, float input);

#endif /* EFFECTS_COMPONENTS_LEAKY_INTEGRATOR_H_ */
