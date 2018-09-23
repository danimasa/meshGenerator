#include <cmath>
#include "Vector.hpp"

namespace geomlib {

double Vector::angleWith(Vector &v) const {
    double uTimesV = x*v.x + y*v.y + z*v.z;
    double timesNorm = norm() * v.norm();
    return acos(uTimesV / timesNorm);
}

double Vector::norm() const {
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector Vector::vectorProduct(const Vector &v) {
    auto a = y * v.z - z * v.y;
    auto b = z * v.x - x * v.z;
    auto c = x * v.y - y * v.x;
    return Vector(a, b, c);
}

}