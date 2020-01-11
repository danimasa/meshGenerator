#pragma once

#include "Geometry.hpp"

namespace meshlib {

using namespace geomlib;

class Quadrilateral : public Geometry {
public:
    Quadrilateral(Vertex *v1, Vertex *v2, Vertex *v3, Vertex *v4) :
        Geometry(GeometryType::Element),
        v1(v1), v2(v2), v3(v3), v4(v4) {}

    GeometryType getGeometryType() const { return GeometryType::Element; }

    Vertex *v1, *v2, *v3, *v4;
};

}