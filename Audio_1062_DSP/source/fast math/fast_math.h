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
extern const float ln2;
extern const float logE;
float fastLN(float x);
float fastExp(float x);
#define fastPow10(x) fastExp(ln10 * x)
#define fastPow10(x) fastExp(ln10 * x)
#define fastLog10(x) (logE * fastLN(x))
#define fastPow2(x) fastExp(ln2 * x)

#endif /* FAST_MATH_FAST_MATH_H_ */
