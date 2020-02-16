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

bool Plane::contains(const Line* l) {
    Vector v(l->init_point, l->final_point);
    return contains(&v);
}

Vector Plane::normalVector() const {
    return vector1->vectorProduct(*vector2);
}

double Plane::angleBetween(const Vector &v1, const Vector &v2) const {
    double dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    Vector n = normalVector();
    n.normalise();
    double det = v1.x * v2.y * n.z
        + v2.x * n.y * v1.z
        + n.x * v1.y * v2.z
        - v1.z * v2.y * n.x
        - v2.z * n.y * v1.x
        - n.z * v1.y * v2.x;
    return atan2(det, dot);
}

}