#include <catch.hpp>

#include "Polyline.hpp"
#include "StraightLine.hpp"
#include "GeometryFactory.hpp"

using namespace geomlib;

TEST_CASE("Polyline") {
    const GeometryFactory* factory = GeometryFactory::getDefaultInstance();

    // Valid Polylines
    SECTION("All with same direction") {
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
    }

    SECTION("Inverted directions") {
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
        auto line2 = factory->createStraightLine(midKp2, midKp1);
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
    }

    // Invalid Polylines
    SECTION("Not a loop (init and final point equal)") {
        Point initPoint(0, 0, 0);
        Point midPoint1(0, 1, 0);
        Point midPoint2(1, 0, 0);

        auto initKp = factory->createKeypoint(initPoint);
        auto midKp1 = factory->createKeypoint(midPoint1);
        auto midKp2 = factory->createKeypoint(midPoint2);

        auto line1 = factory->createStraightLine(initKp, midKp1);
        auto line2 = factory->createStraightLine(midKp1, midKp2);
        auto line3 = factory->createStraightLine(midKp2, initKp);

        vector<Line*> lines;
        lines.push_back(line1);
        lines.push_back(line2);
        lines.push_back(line3);

        REQUIRE_THROWS( factory->createPolyline(initKp, initKp, lines) );
    }
    
    SECTION("Not touching lines") {
        Point initPoint(0, 0, 0);
        Point midPoint1(0, 1, 0);
        Point midPoint2(1, 1, 0);
        Point finalPoint(1, 0, 0);

        auto initKp = factory->createKeypoint(initPoint);
        auto midKp1 = factory->createKeypoint(midPoint1);
        auto midKp2 = factory->createKeypoint(midPoint2);
        auto finalKp = factory->createKeypoint(finalPoint);

        auto line1 = factory->createStraightLine(initKp, midKp1);
        auto line2 = factory->createStraightLine(midKp2, finalKp);

        vector<Line*> lines;
        lines.push_back(line1);
        lines.push_back(line2);

        REQUIRE_THROWS( factory->createPolyline(initKp, finalKp, lines) );
    }

    SECTION("Without continuity") {
        Point initPoint(0, 0, 0);
        Point midPoint1(0, 1, 0);
        Point finalPoint(1, 0, 0);

        auto initKp = factory->createKeypoint(initPoint);
        auto midKp1 = factory->createKeypoint(midPoint1);
        auto finalKp = factory->createKeypoint(finalPoint);

        auto line1 = factory->createStraightLine(initKp, finalKp);
        auto line2 = factory->createStraightLine(midKp1, initKp);

        vector<Line*> lines;
        lines.push_back(line1);
        lines.push_back(line2);

        REQUIRE_THROWS( factory->createPolyline(initKp, finalKp, lines) );
    }

    SECTION("At least 2 lines") {
        Point initPoint(0, 0, 0);
        Point finalPoint(1, 0, 0);

        auto initKp = factory->createKeypoint(initPoint);
        auto finalKp = factory->createKeypoint(finalPoint);

        auto line1 = factory->createStraightLine(initKp, finalKp);

        vector<Line*> lines;
        lines.push_back(line1);

        REQUIRE_THROWS( factory->createPolyline(initKp, finalKp, lines) );
    }

    SECTION("Initial or final point not find in line list") {
        Point initPoint(0, 0, 0);
        Point midPoint1(0, 1, 0);
        Point midPoint2(0, 1, 0);
        Point finalPoint(1, 0, 0);

        auto initKp = factory->createKeypoint(initPoint);
        auto midKp1 = factory->createKeypoint(midPoint1);
        auto midKp2 = factory->createKeypoint(midPoint2);
        auto finalKp = factory->createKeypoint(finalPoint);

        auto line1 = factory->createStraightLine(midKp1, midKp2);
        auto line2 = factory->createStraightLine(midKp2, finalKp);

        vector<Line*> lines;
        lines.push_back(line1);
        lines.push_back(line2);

        REQUIRE_THROWS( factory->createPolyline(initKp, finalKp, lines) );
        REQUIRE_THROWS( factory->createPolyline(finalKp, initKp, lines) );
    }
}