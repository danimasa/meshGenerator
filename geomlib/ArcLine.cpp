#define _USE_MATH_DEFINES
#include "ArcLine.hpp"
#include "mathUtils.hpp"
#include "GeometryFactory.hpp"

#include <cmath>
#include "armadillo"

namespace geomlib
{

ArcLine::ArcLine(
    Plane* plane,
    KeyPoint* init_point,
    KeyPoint* final_point,
    double radius,
    Point* center,
    Point* mid_point,
    bool inverted_direction) :
    Line(init_point, final_point),
    plane(plane),
    radius(radius),
    center(center),
    inverted_direction(inverted_direction)
{
    Vector v1(center, init_point);
    Vector v2(center, mid_point);
    auto dot = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
    auto lenSq1 = v1.x*v1.x + v1.y*v1.y + v1.z*v1.z;
    angle = acos(dot/lenSq1) * 2;
    
    auto n = plane->normalVector();
    v90 = v1.vectorProduct(n);
    v90 = (radius / v90.norm()) * v90;
    vCI = v1;
    midPoint = *mid_point;
}

double ArcLine::length() const
{
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
    Vector v2(center, &midPoint);
    Vector v3(center, &point);

    double a_v1_v2 = plane->angleBetween(v1, v2) * 2;
    double a_v1_v3 = plane->angleBetween(v1, v3);

    if(a_v1_v2 < 0) {
        a_v1_v2 = a_v1_v2 * -1;
        a_v1_v3 = a_v1_v3 * -1;
    }

    if(a_v1_v3 < 0)
        a_v1_v3 = 2 * M_PI + a_v1_v3;

    if( double_equals(radical, pow(radius, 2)) &&
        a_v1_v3 < a_v1_v2) {
        double position = a_v1_v3 / a_v1_v2;
        return inverted_direction ? 1 - position : position;
    }

    return -1;
}

Point ArcLine::pointAtPosition(const double position)
{
    if (position < 0.0 || position > 1.0)
    {
        throw std::invalid_argument("pointInLine so aceita valores entre 0 e 1");
    }

    if (double_equals(position, 0.0))
    {
        return *init_point;
    }

    if (double_equals(position, 1.0))
    {
        return *final_point;
    }

    auto beta = angle * position;
    auto vP = vCI * cos(beta) + v90 * sin(beta);
    return *center + vP;

    // double alfa = toInitPoint.angleWith(toFinalPoint);
    // double beta = alfa * position;

    // double w = radius * std::cos(beta);
    // double k = radius * std::sin(beta);

    // Vector orthoVector = normalVector.vectorProduct(toInitPoint);

    // orthoVector.normalise();
    // toFinalPoint.normalise();
    // normalVector.normalise();

    // arma::mat A;
    // A << orthoVector.x << toFinalPoint.x << normalVector.x << arma::endr
    //   << orthoVector.y << toFinalPoint.y << normalVector.y << arma::endr
    //   << orthoVector.z << toFinalPoint.z << normalVector.z << arma::endr;

    // arma::vec vec(3);
    // vec(0) = k;
    // vec(1) = w;
    // vec(2) = 0;
    
    // auto resultVector = A * vec;

    // arma::vec centerVector(3);
    // centerVector(0) = center->x;
    // centerVector(1) = center->y;
    // centerVector(2) = center->z;

    // arma::vec otherVector = resultVector + centerVector;

    // return Point(otherVector(0), otherVector(1), otherVector(2));
}

Box ArcLine::outBox() {
    Vector X(1, 0, 0);
    Vector Y(0, 1, 0);
    Vector Z(0, 0, 1);

    double max_x, max_y, max_z;
    double min_x, min_y, min_z;

    // double max_x_bd, max_y_bd, max_z_bd;
    // double min_x_bd, min_y_bd, min_z_bd;

    max_x = init_point->x >= final_point->x 
        ? init_point->x
        : final_point->x;

    min_x = init_point->x <= final_point->x
        ? init_point->x
        : final_point->x;

    max_y = init_point->y >= final_point->y 
        ? init_point->y
        : final_point->y;

    min_y = init_point->y <= final_point->y
        ? init_point->y
        : final_point->y;

    max_z = init_point->z >= final_point->z 
        ? init_point->z
        : final_point->z;

    min_z = init_point->z <= final_point->z
        ? init_point->z
        : final_point->z;

    Vector n1 = X.vectorProduct(plane->normalVector());
    if (!n1.isNull()) {
        n1.normalise();
        Point p1_yz = (n1 * radius) + Vector(*center);
        Point p2_yz = (n1 * (radius * -1)) + Vector(*center);

        double p1_yz_pos = isPointInLine(p1_yz);
        double p2_yz_pos = isPointInLine(p2_yz);

        bool p1_yz_in = p1_yz_pos >= 0 && p1_yz_pos <= 1;
        bool p2_yz_in = p2_yz_pos >= 0 && p2_yz_pos <= 1;

        if(p1_yz.y >= p2_yz.y) {
            if (p1_yz_in && p1_yz.y > max_y)
                max_y = p1_yz.y;
            if (p2_yz_in && p2_yz.y < min_y)
                min_y = p2_yz.y;
        } else {
            if (p2_yz_in && p2_yz.y > max_y)
                max_y = p2_yz.y;
            if (p1_yz_in && p1_yz.y < min_y)
                min_y = p1_yz.y;
        }

        if(p1_yz.z >= p2_yz.z) {
            if (p1_yz_in && p1_yz.z > max_z)
                max_z = p1_yz.z;
            if (p2_yz_in && p2_yz.z < min_z)
                min_z = p2_yz.z;
        } else {
            if (p2_yz_in && p2_yz.z > max_z)
                max_z = p2_yz.z;
            if (p1_yz_in && p1_yz.z < min_z)
                min_z = p1_yz.z;
        }
    }

    Vector n2 = Y.vectorProduct(plane->normalVector());
    if (!n2.isNull()) {
        n2.normalise();
        Point p1_xz = (n2 * radius) + Vector(*center);
        Point p2_xz = (n2 * (radius * -1)) + Vector(*center);

        double p1_xz_pos = isPointInLine(p1_xz);
        double p2_xz_pos = isPointInLine(p2_xz);

        bool p1_xz_in = p1_xz_pos >= 0 && p1_xz_pos <= 1;
        bool p2_xz_in = p2_xz_pos >= 0 && p2_xz_pos <= 1;

        if(p1_xz.x >= p2_xz.x) {
            if(p1_xz_in && p1_xz.x > max_x)
                max_x = p1_xz.x;
            if(p2_xz_in && p2_xz.x < min_x)
                min_x = p2_xz.x;
        } else {
            if(p2_xz_in && p2_xz.x > max_x)
                max_x = p2_xz.x;
            if(p1_xz_in && p1_xz.x < min_x)
                min_x = p1_xz.x;
        }

        if(p1_xz.z >= p2_xz.z) {
            if(p1_xz_in && p1_xz.z > max_z)
                max_z = p1_xz.z;
            if(p2_xz_in && p2_xz.z < min_z)
                min_z = p2_xz.z;
        } else {
            if(p2_xz_in && p2_xz.z > max_z)
                max_z = p2_xz.z;
            if(p1_xz_in && p1_xz.z < min_z)
                min_z = p1_xz.z;
        }
    }

    if (n1.isNull() || n2.isNull()) {
        Vector n3 = Z.vectorProduct(plane->normalVector());
        n3.normalise();
        Point p1_xy = (n3 * radius) + Vector(*center);
        Point p2_xy = (n3 * (radius * -1)) + Vector(*center);

        double p1_xy_pos = isPointInLine(p1_xy);
        double p2_xy_pos = isPointInLine(p2_xy);

        bool p1_xy_in = p1_xy_pos >= 0 && p1_xy_pos <= 1;
        bool p2_xy_in = p2_xy_pos >= 0 && p2_xy_pos <= 1;

        if(p1_xy.x >= p2_xy.x) {
            if(p1_xy_in && p1_xy.x > max_x)
                max_x = p1_xy.x;
            if(p2_xy_in && p2_xy.x < min_x)
                min_x = p2_xy.x;
        } else {
            if(p2_xy_in && p2_xy.x > max_x)
                max_x = p2_xy.x;
            if(p1_xy_in && p1_xy.x < min_x)
                min_x = p1_xy.x;
        }

        if(p1_xy.y >= p2_xy.y) {
            if(p1_xy_in && p1_xy.y > max_y)
                max_y = p1_xy.y;
            if(p2_xy_in && p2_xy.y < min_y)
                min_y = p2_xy.y;
        } else {
            if(p2_xy_in && p2_xy.y > max_y)
                max_y = p2_xy.y;
            if(p1_xy_in && p1_xy.y < min_y)
                min_y = p1_xy.y;
        }
    }

    double width = max_x - min_x;
    double height = max_y - min_y;
    double depth = max_z - min_z;

    double mid_x = min_x + width / 2;
    double mid_y = min_y + height / 2;
    double mid_z = min_z + depth / 2;

    Point midPoint(mid_x, mid_y, mid_z);
    
    return Box(midPoint, width, height, depth);
}

Vector ArcLine::get_init_vector() const {
    Vector toInitPoint(center, init_point);
    Vector result = toInitPoint.vectorProduct(plane->normalVector());
    result.normalise();
    return result;
}

Vector ArcLine::get_final_vector() const {
    Vector toFinalPoint(center, final_point);
    Vector result = toFinalPoint.vectorProduct(plane->normalVector());
    result.normalise();
    return result;
}

} // namespace geomlib