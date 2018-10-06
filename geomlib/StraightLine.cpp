#include <cmath>
#include <stdexcept>

#include "StraightLine.hpp"

using namespace std;

namespace geomlib {

double StraightLine::length() const {
    return sqrt(pow(final_point->x - init_point->x, 2)
        + pow(final_point->y - init_point->y, 2)
        + pow(final_point->z - init_point->z, 2));
}

Point StraightLine::pointInLine(const double position) {
    if (position < 0 || position > 1) {
        throw std::invalid_argument("pointInLine so aceita valores entre 0 e 1");
    }
    
    if (position == 0) {
        return *init_point;
    }

    if (position == 1) {
        return *final_point;
    }

    auto newX = (final_point->x - init_point->x) * position + init_point->x;
    auto newY = (final_point->y - init_point->y) * position + init_point->y;
    auto newZ = (final_point->z - init_point->z) * position + init_point->z;
    return Point(newX, newY, newZ);
}

}