#include "Plane.hpp"

#include <boost/qvm/mat_operations.hpp>

#define ERRO_ACEITAVEL 0.00000000000000001

namespace geomlib {

bool Plane::contains(const Point* p) {
    Vector v(this->point, p);
    return contains(&v);
}

bool Plane::contains(const Vector* v) {
    double mat[3][3] = {
        {v->x, v->y, v->z},
        {vector1->x, vector1->y, vector1->z},
        {vector2->x, vector2->y, vector2->z}
    };
    double det = boost::qvm::determinant<double[3][3]>(mat);
    if(det < ERRO_ACEITAVEL)
        return true;
    return false;
}

Vector Plane::normalVector() const {
    return vector1->vectorProduct(*vector2);
}

}