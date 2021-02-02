#pragma once

#include "Line.hpp"

namespace geomlib {

class GeometryFactory;

class StraightLine : public Line {
public:
    LineType getLineType() const { return LineType::StraightLine; }
    double length() const;
    Point pointAtPosition(const double position);
    double isPointInLine(const Point &point);
    Box outBox();

    Vector get_init_vector() const;
    Vector get_final_vector() const;

    inline vector<Area*> getAttachedAreas() const { return attachedAreas; }

    int getElementsQty() { return elementsQty; }
    void setElementsQty(int elements) { elementsQty = elements; }

private:
    StraightLine(KeyPoint* init_point, KeyPoint* final_point) :
        Line(init_point, final_point) {}

    Vector get_direction_vector() const;

    friend class GeometryFactory;
};

}