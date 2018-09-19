#pragma once

#include "Line.hpp"

namespace geomlib {

class GeometryFactory;

class UnspecifiedLine : public Line {
public:
    Point* mid_point;
    Vector* tangent_init_vector;
    Vector* tangent_final_vector;

    string getLineType() const { return "unspecified-line"; }
    double length() const { return 0; }

private:
    UnspecifiedLine(
        KeyPoint* init_point,
        KeyPoint* final_point,
        Point* mid_point,
        Vector* tangent_init_vector,
        Vector* tangent_final_vector) :
            Line(init_point, final_point),
            mid_point(mid_point),
            tangent_init_vector(tangent_init_vector),
            tangent_final_vector(tangent_final_vector) {}

    friend class GeometryFactory;
};

}