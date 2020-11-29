#include "Point.hpp"
#include <math.h>

namespace geomlib {

double Point::distance(const Point *p) {
    using namespace std;
    return sqrt(pow(p->x - x, 2) + pow(p->y - y, 2) + pow(p->z - z, 2));
}

Point operator+(const Point &p1, const Point &p2) {
    double x = p1.x + p2.x;
    double y = p1.y + p2.y;
    double z = p1.z + p2.z;
    return Point(x, y, z);
}

Point operator-(const Point &p1, const Point &p2) {
    double x = p1.x - p2.x;
    double y = p1.y - p2.y;
    double z = p1.z - p2.z;
    return Point(x, y, z);
}

}