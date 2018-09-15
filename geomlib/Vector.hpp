#pragma once

#include "Point.hpp"

namespace geomlib {

    class Vector : public Point {
    public:
        // Null vector
        Vector() : Point() {}

        Vector(const double x, const double y, const double z) :
            Point(x, y, z) {}

        Vector(const Point* p1, const Point* p2) :
            Point(p2->x - p1->x, p2->y - p1->y, p2->z - p1->z) {}
    };
}
