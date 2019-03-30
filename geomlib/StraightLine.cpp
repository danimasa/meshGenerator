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

double StraightLine::isPointInLine(const Point &point) {
    if(point == *init_point) return 0;
    if(point == *final_point) return 1;

    Vector v1 (init_point, final_point);
    Vector v2 (init_point, &point);

    double initDistance = init_point->distance(&point);
    double l = length();
    double angle = v1.angleWith(v2);
    if( double_equals(v1.angleWith(v2), 0)
        && initDistance <= l
        && final_point->distance(&point) <= l )
        return initDistance / l;
    
    return -1;
}

Point StraightLine::pointAtPosition(const double position) {
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