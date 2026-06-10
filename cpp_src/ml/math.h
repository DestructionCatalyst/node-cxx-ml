#ifndef ML_MATH_H
#define ML_MATH_H
#include <random>
#include <cmath>

namespace ml {
    const float EPSILON = 0.0000002f;

    size_t random(size_t min, size_t max);
    size_t random(size_t min, size_t max, size_t seed);
}

#endif
