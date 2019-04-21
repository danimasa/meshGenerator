#define _USE_MATH_DEFINES
#include "ArcLine.hpp"
#include "Vector.hpp"
#include "mathUtils.hpp"
#include "GeometryFactory.hpp"

#include <cmath>
#include "armadillo"

namespace geomlib
{

double ArcLine::length() const
{
    Vector toInitPoint(center, init_point);
    Vector toFinalPoint(center, final_point);

    double angle = toInitPoint.angleWith(toFinalPoint);
    return radius * angle;
}

double ArcLine::isPointInLine(const Point &point) {
    if( point == *init_point ) return 0;
    if( point == *final_point ) return 1;

    if (!plane->contains(&point))
        return -1;

    double radical = pow(point.x - center->x, 2) 
                   + pow(point.y - center->y, 2)
                   + pow(point.z - center->z, 2);
    
    Vector v1(center, init_point);
    Vector v2(center, final_point);
    Vector v3(center, &point);
    auto v4 = v1.vectorProduct(plane->normalVector());

    double a_v1_v2 = v1.angleWith(v2);
    double a_v1_v3 = v1.angleWith(v3);
    double a_v2_v3 = v2.angleWith(v3);

    if(v4.angleWith(v3) > M_PI / 2)
        a_v1_v3 += M_PI;

    if(v4.angleWith(v2) > M_PI / 2)
        a_v1_v2 += M_PI;

    if( double_equals(radical, pow(radius, 2)) &&
        a_v1_v3 < a_v1_v2 &&
        a_v2_v3 < a_v1_v2) {
        return a_v1_v3 / a_v1_v2;
    }

    return -1;
}

Point ArcLine::pointAtPosition(const double position)
{
    if (position < 0 || position > 1)
    {
        throw std::invalid_argument("pointInLine so aceita valores entre 0 e 1");
    }

    if (position == 0)
    {
        return *init_point;
    }

    if (position == 1)
    {
        return *final_point;
    }

    Vector toInitPoint(init_point, center);
    Vector toFinalPoint(final_point, center);
    double alfa = toInitPoint.angleWith(toFinalPoint);
    double beta = alfa * position;

    double w = radius * std::cos(beta);
    double k = radius * std::sin(beta);

    Vector normalVector = plane->normalVector();
    Vector orthoVector = normalVector.vectorProduct(toInitPoint);

    orthoVector = orthoVector.normalise();
    toFinalPoint = toFinalPoint.normalise();
    normalVector = normalVector.normalise();

    arma::mat A;
    A << orthoVector.x << toFinalPoint.x << normalVector.x << arma::endr
      << orthoVector.y << toFinalPoint.y << normalVector.y << arma::endr
      << orthoVector.z << toFinalPoint.z << normalVector.z << arma::endr;

    arma::vec vec(3);
    vec(0) = k;
    vec(1) = w;
    vec(2) = 0;

    auto resultVector = A * vec;

    arma::vec centerVector(3);
    centerVector(0) = center->x;
    centerVector(1) = center->y;
    centerVector(2) = center->z;

    arma::vec otherVector = resultVector + centerVector;

    return Point(otherVector(0), otherVector(1), otherVector(2));
}

} // namespace geomlib