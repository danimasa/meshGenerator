#pragma once

#include <math.h>
#include <vector>

static inline bool double_equals(double a, double b, double epsilon = 0.00000001) {
    return std::abs(a - b) < epsilon;
}

enum class APROX_DIRECTION { BOTH, UP, DOWN };

/** 
 * Receives a int approximation and find the index and 
 *   value of the less error approximation change value
 *
 * @param values pair of approximated values and original values
 * @param range: the range to variate the value for approximation
 * 
 * @return index and new lessError approximation value
 */
std::pair<int, int> lessErrorApproximation(
    std::vector<std::pair<int, double>> values,
    APROX_DIRECTION direction
);