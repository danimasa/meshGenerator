#include "Point.hpp"
#include <math.h>

namespace geomlib {

double Point::distance(const Point &p) {
    using namespace std;
    return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2) + pow(p.z - z, 2));
}

}