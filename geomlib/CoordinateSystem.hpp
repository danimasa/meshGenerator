#pragma once

#include "Vector.hpp"

namespace geomlib {

class GeometryFactory;

class CoordinateSystem {
public:
    Point convert(const Point* p) const;
    Point convert(const Point* p, const CoordinateSystem* coord) const;

private:
    Vector* vector1;
    Vector* vector2;
    Vector* vector3;

    CoordinateSystem(Vector* v1, Vector* v2, Vector* v3) :
        vector1(v1), vector2(v2), vector3(v3) {}
    
    friend class GeometryFactory;
};

}