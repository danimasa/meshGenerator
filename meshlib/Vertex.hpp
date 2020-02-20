#pragma once

#include "Geometry.hpp"
#include "Point.hpp"

namespace meshlib {

using namespace geomlib;

class Vertex : public Geometry, public Point {
public:
    Vertex() : Geometry(GeometryType::Vertex), Point() {}

    Vertex(Point &point) :
        Geometry(GeometryType::Vertex),
        Point(point.x, point.y, point.z) {}

    Vertex(double x, double y, double z) :
        Geometry(GeometryType::Vertex),
        Point(x, y, z) {}

    GeometryType getGeometryType() const { return GeometryType::Vertex; }

    bool operator==(const Vertex &that) const {
        return that.getID() == getID();
    }

    bool operator!=(const Vertex &that) const {
        return that.getID() != getID();
    }
};

}