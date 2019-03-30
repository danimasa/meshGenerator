#pragma once

#include "Line.hpp"

namespace geomlib {

class GeometryFactory;

class UnspecifiedLine : public Line {
public:
    Point* mid_point;
    Vector* tangent_init_vector;
    Vector* tangent_final_vector;

    LineType getLineType() const { return LineType::UnspecifiedLine; }
    double length() const { return p_length; }
    Point pointAtPosition(const double position) { return Point(); }
    double isPointInLine(const Point &point) { return -1; }

private:
    UnspecifiedLine(
        KeyPoint* init_point,
        KeyPoint* final_point,
        Point* mid_point,
        Vector* tangent_init_vector,
        Vector* tangent_final_vector,
        double length) :
            Line(init_point, final_point),
            mid_point(mid_point),
            tangent_init_vector(tangent_init_vector),
            tangent_final_vector(tangent_final_vector),
            p_length(length) {}

    double p_length;

    friend class GeometryFactory;
};

}