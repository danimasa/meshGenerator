#pragma once

#include "Geometry.hpp"
#include "Point.hpp"

namespace geomlib {

    class KeyPoint : public Geometry, public Point {
    public:
        KeyPoint() : Point() {}

        KeyPoint(Point &point) :
            Point(point.x, point.y, point.z) {}

        std::string getGeometryType() const { return "keypoint"; }
    };
}
