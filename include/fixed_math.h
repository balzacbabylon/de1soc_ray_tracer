#ifndef FIXED_MATH_H
#define FIXED_MATH_H

#include "common.h"

#define FIXED_ONE (1 << 16)
#define INT_TO_FIXED(x) ((x) << 16)
#define FIXED_TO_INT(x) ((x) >> 16)
#define FLOAT_TO_FIXED(f) ((fixed)((f) * 65536.0f))

// TODO: There is no overflow/underflow checking...need to add that
static inline fixed mul_fixed(fixed a, fixed b) {
    return (fixed)(((int64_t)a * b) >> 16);
}

static inline fixed div_fixed(fixed a, fixed b) {
    return (fixed)(((int64_t)a << 16) / (int64_t)b);
}

#endif // FIXED_MATH_H