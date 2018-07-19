#pragma once

#include "Identifiable.hpp"
#include "Point.hpp"

class KeyPoint : public Identifiable, public Point {
public:
    KeyPoint() : Point() {}

    KeyPoint(const double x, const double y, const double z) :
        Point(x, y, z) {}

    KeyPoint(const int id, const double x, const double y, const double z) :
        Identifiable(id), Point(x, y, z) {}
};