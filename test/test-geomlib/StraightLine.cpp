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

    SECTION("is Point in Line") {
        Point inLinePoint(0, 0, 0.75);
        REQUIRE( line->isPointInLine(inLinePoint) == true );

        Point outLineLowerPoint(0, 0, -1);
        REQUIRE(line->isPointInLine(outLineLowerPoint) == false );

        Point outLineUpperPoint(0, 0, 1.5);
        REQUIRE(line->isPointInLine(outLineUpperPoint) == false );

        Point final3dPoint(1, 1, 1);
        KeyPoint* final3dKp = factory->createKeypoint(final3dPoint);
        StraightLine* line3d = factory->createStraightLine(initKp, final3dKp);
        Point in3dLinePoint(0.5, 0.5, 0.5);
        REQUIRE( line3d->isPointInLine(in3dLinePoint) == true );

        Point notInLinePoint(2, 0, 1.5);
        REQUIRE( line3d->isPointInLine(notInLinePoint) == false );
    }
}