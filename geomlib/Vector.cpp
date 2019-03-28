#include <cmath>
#include "Vector.hpp"
#include "mathUtils.hpp"

#include "armadillo"

namespace geomlib {

double Vector::angleWith(Vector &v) const {
    double uTimesV = x*v.x + y*v.y + z*v.z;
    double timesNorm = norm() * v.norm();
    double division = uTimesV / timesNorm;
    // tratar problema de valor maior que 1
    if(double_equals(division, 1)) division = 1;
    return acos(division);
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

Vector Vector::normalise() {
    arma::vec vec(3);
    vec(0) = x;
    vec(1) = y;
    vec(2) = z;

    arma::vec normalized = arma::normalise(vec);
    return Vector(normalized(0), normalized(1), normalized(2));
}

}