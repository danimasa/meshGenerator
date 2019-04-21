#define _USE_MATH_DEFINES
#include <catch.hpp>

#include "Vector.hpp"

using namespace geomlib;

TEST_CASE("Vector") {
    // Norma
    Vector v(1, 0, 0);
    REQUIRE( v.norm() == 1.0 );

    // Angle With
    Vector v1(0, 1, 0);
    double angle = v.angleWith(v1);
    REQUIRE( angle == M_PI / 2 );

    // Produto Vetorial
    Vector v3 = v.vectorProduct(v1);
    REQUIRE( v3.angleWith(v) == M_PI / 2 );
    REQUIRE( v3.angleWith(v1) == M_PI / 2 );

    // Normalizacao
    Vector v4(2, 0, 0);
    REQUIRE( v4.normalise() == v );
}