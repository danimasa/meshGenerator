#pragma once

namespace geomlib {

    class Point {
    public:
        double x, y, z;

        Point() : x(0), y(0), z(0) {}

        Point(const double x, const double y, const double z) :
            x(x), y(y), z(z) {}

        double distance(const Point *p);

        bool operator==(const Point &that) const {
            bool isEqual = false;
            if(that.x == x && that.y == y && that.z == z) {
                isEqual = true;
            }
            return isEqual;
        }
    };

}
