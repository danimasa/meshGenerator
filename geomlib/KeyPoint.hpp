#pragma once

#include "Geometry.hpp"
#include "Point.hpp"

namespace geomlib {

class GeometryFactory;

enum class VertexType {
    ORTHOGONAL,
    POLAR
};

class KeyPoint : public Geometry, public Point {
public:
    GeometryType getGeometryType() const { return GeometryType::Keypoint; }

    bool operator==(const KeyPoint &that) const {
        return that.getID() == getID();
    }

    bool operator!=(const KeyPoint &that) const {
        return that.getID() != getID();
    }

    VertexType type;

private:
    KeyPoint() : Geometry(GeometryType::Keypoint), Point() {}

    KeyPoint(Point &point) :
        Geometry(GeometryType::Keypoint),
        Point(point.x, point.y, point.z) {}

    friend class GeometryFactory;
};
}
