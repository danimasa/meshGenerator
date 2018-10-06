#include "ArcLine.hpp"
#include "Vector.hpp"

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

Point ArcLine::pointInLine(const double position)
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