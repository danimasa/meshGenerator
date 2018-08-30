#pragma once

#include "Point.hpp"

namespace geomlib {

class GeometryUtils {
public:
    static double distanceBetweenPoints(const Point* p1, const Point* p2);

private:
    GeometryUtils() {}
};

}