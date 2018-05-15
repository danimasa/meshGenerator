#pragma once

struct KeyPoint {
    double x, y, z;

    KeyPoint() : x(0), y(0), z(0) {}

    KeyPoint(const double x, const double y, const double z) :
        x(x), y(y), z(z) {}
};