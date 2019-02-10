#include <catch.hpp>

#include "Polyline.hpp"
#include "StraightLine.hpp"
#include "GeometryFactory.hpp"

using namespace geomlib;

TEST_CASE("Polyline") {
    const GeometryFactory* factory = GeometryFactory::getDefaultInstance();

    SECTION("Valid polyline") {
        // All with same direction
        Point initPoint(0, 0, 0);
        Point midPoint1(0, 1, 0);
        Point midPoint2(2, 1, 0);
        Point finalPoint(2, 0, 0);
        Point midPolylinePoint(1, 1, 0);

        auto initKp = factory->createKeypoint(initPoint);
        auto midKp1 = factory->createKeypoint(midPoint1);
        auto midKp2 = factory->createKeypoint(midPoint2);
        auto finalKp = factory->createKeypoint(finalPoint);

        auto line1 = factory->createStraightLine(initKp, midKp1);
        auto line2 = factory->createStraightLine(midKp1, midKp2);
        auto line3 = factory->createStraightLine(midKp2, finalKp);

        vector<Line*> lines;
        lines.push_back(line1);
        lines.push_back(line2);
        lines.push_back(line3);

        Polyline* polyline = factory->createPolyline(initKp, finalKp, lines);

        REQUIRE( polyline->length() == 4.0 );
        REQUIRE_THROWS( polyline->pointInLine(-0.1) );
        REQUIRE_THROWS( polyline->pointInLine(1.1) );

        REQUIRE( polyline->pointInLine(0) == initPoint );
        REQUIRE( polyline->pointInLine(0.5) == midPolylinePoint);
        REQUIRE( polyline->pointInLine(0.25) == midPoint1);
        REQUIRE( polyline->pointInLine(0.75) == midPoint2);
        REQUIRE( polyline->pointInLine(1) == finalPoint );

        // Inverted directions
    }

    SECTION("Invalid polyline") {
        // Not touching lines

        // Starts and Ends in same point
    }
}