/*
 * filter_coefficients.h
 *
 *  Created on: Oct 1, 2024
 *      Author: isaac
 */

#ifndef EFFECTS_FILTERS_FILTER_COEFFICIENTS_H_
#define EFFECTS_FILTERS_FILTER_COEFFICIENTS_H_

typedef struct {
	float a0;
	float a1;
	float a2;
	float b0;
	float b1;
	float b2;
} FILTER_COEF;

#endif /* EFFECTS_FILTERS_FILTER_COEFFICIENTS_H_ */
