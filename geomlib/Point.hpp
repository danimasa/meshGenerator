#pragma once

#include <cmath>
#include <algorithm>

namespace geomlib {
    class Point {
    public:
        double x, y, z;

        Point() : x(0), y(0), z(0) {}

        Point(const double x, const double y, const double z) :
            x(x), y(y), z(z) {}

        double distance(const Point *p);

        bool operator==(const Point &that) const {
            return isEqualPoint(that);
        }

        bool operator!=(const Point &that) const {
            return !isEqualPoint(that);
        }

        friend Point operator+(const Point &p1, const Point &p2);
        friend Point operator-(const Point &p1, const Point &p2);
    private:
        bool isEqualPoint(const Point &that) const {
            bool isEqual = false;
                
            if((that.x == x || std::abs(that.x-x)<std::abs(std::min(that.x,x))*0.00000001) && 
                (that.y == y || std::abs(that.y-y)<std::abs(std::min(that.y,y))*0.00000001) && 
                (that.z == z || std::abs(that.z-z)<std::abs(std::min(that.z,z))*0.00000001)) {
                isEqual = true;
            }
            return isEqual;
        }
    };

}
