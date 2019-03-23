#pragma once

#include "Geometry.hpp"
#include "Point.hpp"

namespace geomlib {

    class KeyPoint : public Geometry, public Point {
    public:
        // Contrutores somente para friend class
        KeyPoint() : Geometry(GeometryType::Keypoint), Point() {}

        KeyPoint(Point &point) :
            Geometry(GeometryType::Keypoint),
            Point(point.x, point.y, point.z) {}

        GeometryType getGeometryType() const { return GeometryType::Keypoint; }

        bool operator==(const KeyPoint &that) const {
            return that.getID() == getID();
        }

        bool operator!=(const KeyPoint &that) const {
            return that.getID() != getID();
        }
    };
}
