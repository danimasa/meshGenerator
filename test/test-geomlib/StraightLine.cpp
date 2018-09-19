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
}