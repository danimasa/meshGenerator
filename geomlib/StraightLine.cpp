#include <cmath>

#include "StraightLine.hpp"

using namespace std;

namespace geomlib {

double StraightLine::length() const {
    return sqrt(pow(final_point->x - init_point->x, 2)
        + pow(final_point->y - init_point->y, 2)
        + pow(final_point->z - init_point->z, 2));
}

}