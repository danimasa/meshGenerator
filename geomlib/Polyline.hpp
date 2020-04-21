#pragma once

#include <vector>
#include "Line.hpp"

namespace geomlib {

class GeometryFactory;

class Polyline : public Line {
public:
    LineType getLineType() const { return LineType::Polyline; }

    struct Line_in_Polyline {
        Line* line;
        LineDirection direction;
    };

    std::vector<Line*> get_lines() const;
    std::vector<Line_in_Polyline> getLinesWithDirection() const;
    double length() const;
    Point pointAtPosition(const double position);
    double isPointInLine(const Point &point);
    Box outBox();
    std::vector<KeyPoint*> intermidiaryPoints() const;

    Vector get_init_vector() const;
    Vector get_final_vector() const;

private:
    Polyline(
        KeyPoint* init_point,
        KeyPoint* final_point,
        std::vector<Line*> lineList);

    std::vector<Line_in_Polyline> lines;

    friend class GeometryFactory;
};

}