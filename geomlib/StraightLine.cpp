#include <cmath>
#include <stdexcept>

#include "mathUtils.hpp"
#include "StraightLine.hpp"

using namespace std;

namespace geomlib {

double StraightLine::length() const {
    return sqrt(pow(final_point->x - init_point->x, 2)
        + pow(final_point->y - init_point->y, 2)
        + pow(final_point->z - init_point->z, 2));
}

bool StraightLine::isPointInLine(const Point &point) {
    double mx, my, mz;
    double divx, divy, divz;
    divx = final_point->x - init_point->x;
    divy = final_point->y - init_point->y;
    divz = final_point->z - init_point->z;
    mx = point.x - init_point->x / divx;
    my = point.y - init_point->y / divy;
    mz = point.z - init_point->z / divz;
    return (double_equals(mx, my) || divx == 0) 
        && (double_equals(mx, mz) || divx == 0)
        && (double_equals(my, mz) || divz == 0 || divy == 0)
        && init_point->distance(&point) <= length()
        && final_point->distance(&point) <= length();
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