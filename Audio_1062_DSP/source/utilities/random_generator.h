/*
 * random_generator.h
 *
 *  Created on: Sep 11, 2024
 *      Author: isaac
 */

#ifndef UTILITIES_RANDOM_GENERATOR_H_
#define UTILITIES_RANDOM_GENERATOR_H_

#include "fsl_trng.h"

#define TRNG0 TRNG

void intitialize_random_number_generator(void);
float get_random_float(void);

#endif /* UTILITIES_RANDOM_GENERATOR_H_ */
