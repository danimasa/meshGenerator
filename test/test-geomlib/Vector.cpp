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
}