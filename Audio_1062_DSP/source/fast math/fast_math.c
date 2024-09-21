/*
 * fast_math.c
 *
 *  Created on: Sep 21, 2024
 *      Author: isaac
 */

#include "fast_math.h"

// reasonable (+-3%) and fast approximation of exp(x) for x < 80
float fastExp(float x)
{
    float a = (1 << 23) / 0.69314718f;
    float b = (1 << 23) * (127 - 0.043677448f);
    x = a * x + b;

#if 0
    // Remove these lines if bounds checking is not needed
    constexpr float c = (1 << 23);
    constexpr float d = (1 << 23) * 255;
    if (x < c || x > d)
        x = (x < c) ? 0.0f : d;
#endif
    // With C++20 one can use std::bit_cast instead
    uint32_t n = (uint32_t)(x);
    memcpy(&x, &n, 4);
    return x;
}

const float ln10 = 2.30258509299;

float fastLN(float x) {
  unsigned int bx = * (unsigned int *) (&x);
  unsigned int ex = bx >> 23;
  signed int t = (signed int)ex-(signed int)127;
  unsigned int s = (t < 0) ? (-t) : t;
  bx = 1065353216 | (bx & 8388607);
  x = * (float *) (&bx);
  return -1.49278+(2.11263+(-0.729104+0.10969*x)*x)*x+0.6931471806*t;
}
