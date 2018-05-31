#pragma once

#include "Identifiable.hpp"

struct KeyPoint : public Identifiable {
    double x, y, z;

    KeyPoint() : x(0), y(0), z(0) {}

    KeyPoint(const double x, const double y, const double z) :
        x(x), y(y), z(z) {}

    KeyPoint(const int id, const double x, const double y, const double z) :
        Identifiable(id), x(x), y(y), z(z) {}
};