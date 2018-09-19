#include "ArcLine.hpp"
#include "Vector.hpp"

namespace geomlib {

double ArcLine::length() const {
    Vector toInitPoint(center, init_point);
    Vector toFinalPoint(center, final_point);

    double angle = toInitPoint.angleWith(toFinalPoint);
    return radius * angle;
}

}