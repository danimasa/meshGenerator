#include <catch.hpp>

#include "Point.hpp"

using namespace geomlib;

TEST_CASE("Point") {

    // Distancia entre dois pontos
    Point p1(0, 0, 0);
    Point p2(0, 1, 0);

    REQUIRE( p1.distance(&p2) == 1 );
}