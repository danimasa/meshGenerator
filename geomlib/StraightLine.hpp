#pragma once

#include "Line.hpp"

namespace geomlib {

class GeometryFactory;

class StraightLine : public Line {
public:
    LineType getLineType() const { return LineType::StraightLine; }
    double length() const;
    Point pointInLine(const double position);

private:
    StraightLine(KeyPoint* init_point, KeyPoint* final_point) :
        Line(init_point, final_point) {}

    friend class GeometryFactory;
};

}