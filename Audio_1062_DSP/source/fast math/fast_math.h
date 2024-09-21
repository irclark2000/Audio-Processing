/*
 * fast_math.h
 *
 *  Created on: Sep 21, 2024
 *      Author: isaac
 */

#ifndef FAST_MATH_FAST_MATH_H_
#define FAST_MATH_FAST_MATH_H_

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern const float ln10;
float fastLN(float x);
float fastExp(float x);
#define fastPow10(x) fastExp(ln10 * x)

#endif /* FAST_MATH_FAST_MATH_H_ */
