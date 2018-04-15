#pragma once

class KeyPoint {
public:
    double x, y, z;
    KeyPoint(const double x, const double y, const double z) :
        x(x), y(y), z(z) {}
};