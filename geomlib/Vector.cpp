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

void Vector::normalise() {
    arma::vec vec(3);
    vec(0) = x;
    vec(1) = y;
    vec(2) = z;

    arma::vec normalized = arma::normalise(vec);

    x = normalized(0);
    y = normalized(1);
    z = normalized(2);
}

bool Vector::isNull() {
    return double_equals(x, 0) && double_equals(y, 0) && double_equals(z, 0);
}


Vector operator+(const Vector &v1, const Vector &v2) {
    double x = v1.x + v2.x;
    double y = v1.y + v2.y;
    double z = v1.z + v2.z;
    return Vector(x, y, z);
}

Vector operator-(const Vector &v1, const Vector &v2) {
    double x = v1.x - v2.x;
    double y = v1.y - v2.y;
    double z = v1.z - v2.z;
    return Vector(x, y, z);
}

Vector operator*(const Vector &v1, double value) {
    double x = v1.x * value;
    double y = v1.y * value;
    double z = v1.z * value;
    return Vector(x, y, z);
}
}