#pragma once

#include <vector>
#include "Line.hpp"

namespace geomlib {

class GeometryFactory;

class Polyline : public Line {
public:
    LineType getLineType() const { return LineType::Polyline; }

    std::vector<Line*> get_lines() const { return lines; }
    double length() const { return 0.0; }
    Point pointInLine(const double position) { return Point(0, 0, 0); }

private:
    Polyline(
        KeyPoint* init_point,
        KeyPoint* final_point,
        std::vector<Line*> lines) :
        Line(init_point, final_point),
        lines(lines) {}

    std::vector<Line*> lines;

    friend class GeometryFactory;
};

}