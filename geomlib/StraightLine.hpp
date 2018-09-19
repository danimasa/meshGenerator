#pragma once

#include "Line.hpp"

namespace geomlib {

class GeometryFactory;

class StraightLine : public Line {
public:
    string getLineType() const { return "straight-line"; }
    double length() const;

private:
    StraightLine(KeyPoint* init_point, KeyPoint* final_point) :
        Line(init_point, final_point) {}

    friend class GeometryFactory;
};

}