#pragma once

namespace geomlib {

    class Point {
    public:
        double x, y, z;

        Point() : x(0), y(0), z(0) {}

        Point(const double x, const double y, const double z) :
            x(x), y(y), z(z) {}

        double distance(const Point *p);
    };

}
