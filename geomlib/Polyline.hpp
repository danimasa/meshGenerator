#pragma once

#include <vector>
#include "Line.hpp"

namespace geomlib {

class GeometryFactory;

class Polyline : public Line {
public:
    LineType getLineType() const { return LineType::Polyline; }

    std::vector<Line*> get_lines() const;
    double length() const;
    Point pointAtPosition(const double position);
    double isPointInLine(const Point &point);
    Box outBox();
    std::vector<KeyPoint*> intermidiaryPoints() const;

    struct Line_in_Polyline {
        Line* line;
        LineDirection direction;
    };

private:
    Polyline(
        KeyPoint* init_point,
        KeyPoint* final_point,
        std::vector<Line*> lineList);

    std::vector<Line_in_Polyline> lines;

    friend class GeometryFactory;
};

}