/*
 * leaky_integrator.c
 *
 *  Created on: Nov 1, 2024
 *      Author: isaac
 */

#include "leaky_integrator.h"
void intialize_LEAKY_INTEGRATOR (LEAKY_INTEGRATOR *li, uint8_t N) {
	li->liOut = 0;
	li->Yn_1 = 0;
	li->N = N;
	li->inv_N = 1.0f/N;
}
float apply_LEAKY_INTEGRATOR (LEAKY_INTEGRATOR *li, float input) {
    li->liOut = (li->N * li->Yn_1  + input) * li->inv_N;
    li->Yn_1 = li->liOut;
    return li->liOut;
}
