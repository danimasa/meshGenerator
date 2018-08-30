#include "GeometryUtils.hpp"
#include <math.h>

using namespace std;

namespace geomlib {

double GeometryUtils::distanceBetweenPoints(const Point* p1, const Point* p2) {
    return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2) + pow(p1->z - p2->z, 2));
}

}