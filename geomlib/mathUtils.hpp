#pragma once

#include <math.h>

static inline bool double_equals(double a, double b, double epsilon = 0.00000001) {
    return std::abs(a - b) < epsilon;
}