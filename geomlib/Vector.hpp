#pragma once

#include "Point.hpp"

namespace geomlib {

    class Vector : public Point {
    public:
        // Null vector
        Vector() : Point() {}

        Vector(const double x, const double y, const double z) :
            Point(x, y, z) {}
    };
}
