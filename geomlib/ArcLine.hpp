#pragma once

#include "Line.hpp"
#include "Plane.hpp"

namespace geomlib {

class GeometryFactory;

class ArcLine : public Line {
public:
    string getLineType() const { return "arc-line"; }
    double length() const;
    Point pointInLine(const double position);

    double get_radius() { return radius; }
    Point* get_center() { return center; }

private:
    ArcLine(
        Plane* plane,
        KeyPoint* init_point,
        KeyPoint* final_point,
        double radius,
        Point* center) :
        Line(init_point, final_point),
        plane(plane),
        radius(radius),
        center(center) {}

    Plane* plane;
    double radius;
    Point* center;

    friend class GeometryFactory;
};

}