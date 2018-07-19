#pragma once

#include "Geometry.hpp"
#include "Point.hpp"

namespace geomlib {

    class KeyPoint : public Geometry, public Point {
    public:
        KeyPoint() : Point() {}

        KeyPoint(const double x, const double y, const double z) :
            Point(x, y, z) {}

        std::string getGeometryType() const { return "keypoint"; }
    };
}
