#pragma once

#include "Point.hpp"
#include "Line.hpp"
#include "Vector.hpp"

namespace geomlib {

class GeometryFactory;

class Plane {
public:
    bool contains(const Point* p);
    bool contains(const Vector* v);
    bool contains(const Line* l);

    Vector normalVector() const;
    double angleBetween(const Vector &v1, const Vector &v2) const;

private:
    Point* point;
    Vector* vector1;
    Vector* vector2;

    Plane(Point* p, Vector* v1, Vector* v2) :
        point(p), vector1(v1), vector2(v2) {}

    friend class GeometryFactory;
};

}