#include <catch.hpp>

#include "Plane.hpp"
#include "GeometryFactory.hpp"

using namespace geomlib;

TEST_CASE("Plane") {
    const GeometryFactory* factory = GeometryFactory::getDefaultInstance();

    Vector xVector(1, 0, 0);
    Vector yVector(0, 1, 0);
    Point p(0, 0, 0);

    Plane* plane = factory->createPlane(&p, &xVector, &yVector);

    SECTION("Contains Point") {
        Point p1(2, 3, 0);
        REQUIRE( plane->contains(&p1) == true );

        Point p2(2, 3, 4);
        REQUIRE( plane->contains(&p2) == false );
    }

    SECTION("Contains Vector") {
        Vector vector1(1, 1, 0);
        REQUIRE( plane->contains(&vector1) == true );

        Vector vector2(1, 1, 1);
        REQUIRE( plane->contains(&vector2) == false );
    }
}