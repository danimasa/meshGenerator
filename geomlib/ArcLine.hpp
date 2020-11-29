#pragma once

#include "Line.hpp"
#include "Plane.hpp"
#include "Vector.hpp"

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
    Vector get_final_vector() const;
    Vector get_init_vector() const;

    inline vector<Area*> getAttachedAreas() const { return attachedAreas; }

private:
    ArcLine(
        Plane* plane,
        KeyPoint* init_point,
        KeyPoint* final_point,
        double radius,
        Point* center,
        Point* mid_point,
        bool inverted_direction);

    Plane* plane;
    double radius;
    Point* center;
    Point midPoint;
    double angle;
    Vector v90;
    Vector vCI; // vector from center to init_point
    bool inverted_direction;

    friend class GeometryFactory;
};

}