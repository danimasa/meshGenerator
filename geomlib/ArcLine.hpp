#pragma once

#include "Line.hpp"
#include "Plane.hpp"

namespace geomlib {

class GeometryFactory;

class ArcLine : public Line {
public:
    LineType getLineType() const { return LineType::ArcLine; }
    double length() const;
    Point pointAtPosition(const double position);
    double isPointInLine(const Point &point);
    Box outBox();

    double get_radius() { return radius; }
    Point* get_center() { return center; }
    Plane* get_plane() { return plane; }

private:
    ArcLine(
        Plane* plane,
        KeyPoint* init_point,
        KeyPoint* final_point,
        double radius,
        Point* center,
        bool inverted_direction) :
        Line(init_point, final_point),
        plane(plane),
        radius(radius),
        center(center),
        inverted_direction(inverted_direction) {}

    Plane* plane;
    double radius;
    Point* center;
    bool inverted_direction;

    friend class GeometryFactory;
};

}