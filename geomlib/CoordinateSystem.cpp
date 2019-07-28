#include "CoordinateSystem.hpp"
#include "armadillo"

namespace geomlib {

Point CoordinateSystem::convert(const Point* p) const {
    arma::mat A;
    A << vector1->x << vector2->x << vector3->x << arma::endr
      << vector1->y << vector2->y << vector3->y << arma::endr
      << vector1->z << vector2->z << vector3->z << arma::endr;

    arma::vec vec(3);
    vec(0) = p->x;
    vec(1) = p->y;
    vec(2) = p->z;

    arma::vec resultVector = arma::solve(A, vec);

    return Point(resultVector(0), resultVector(1), resultVector(2));
}

Point CoordinateSystem::convert(const Point* p, const CoordinateSystem* coord) const {
    return *p;
}

}