#include <catch.hpp>

#include "StraightLine.hpp"
#include "GeometryFactory.hpp"

using namespace geomlib;

TEST_CASE("Straight Line") {
    const GeometryFactory* factory = GeometryFactory::getDefaultInstance();

    Point initPoint(0, 0, 0);
    Point finalPoint(0, 0, 1);

    KeyPoint* initKp = factory->createKeypoint(initPoint);
    KeyPoint* finalKp = factory->createKeypoint(finalPoint);

    StraightLine* line = factory->createStraightLine(initKp, finalKp);

    REQUIRE( line->length() == 1.0 );

    SECTION("Point in line") {
        // Somente valores entre 0 e 1
        REQUIRE_THROWS( line->pointInLine(-0.5) );
        REQUIRE_THROWS( line->pointInLine(1.1) );

        REQUIRE( line->pointInLine(0) == initPoint );
        REQUIRE( line->pointInLine(1) == finalPoint );

        Point midPoint(0, 0, 0.5);
        REQUIRE( line->pointInLine(0.5) == midPoint );
    }
}