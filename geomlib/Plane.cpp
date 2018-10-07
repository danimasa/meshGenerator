#include "Plane.hpp"
#include "mathUtils.hpp"

#include "armadillo"

namespace geomlib {

bool Plane::contains(const Point* p) {
    Vector v(this->point, p);
    return contains(&v);
}

bool Plane::contains(const Vector* v) {
    using namespace std;
    double alfa = v->x / vector1->x;
    double teste1 = v->y / vector1->y;
    double teste2 = v->z / vector1->z;

    arma::mat A;
    A << v->x << v->y << v->z << arma::endr
      << vector1->x << vector1->y << vector1->z << arma::endr
      << vector2->x << vector2->y << vector2->z << arma::endr;

    arma::mat L, U, P;
    arma::lu(L, U, P, A);

    if ( ( double_equals(0, U(0, 0)) && double_equals(0, U(0, 1)) && double_equals(0, U(0, 2))) ||
         ( double_equals(0, U(1, 0)) && double_equals(0, U(1, 1)) && double_equals(0, U(1, 2))) ||
         ( double_equals(0, U(2, 0)) && double_equals(0, U(2, 1)) && double_equals(0, U(2, 2))) ) {
    
        return true;
    }

    return false;
}

Vector Plane::normalVector() const {
    return vector1->vectorProduct(*vector2);
}

}