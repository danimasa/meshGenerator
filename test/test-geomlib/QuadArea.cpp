#include <catch.hpp>

#include "QuadArea.hpp"
#include "GeometryFactory.hpp"
#include "StraightLine.hpp"

using namespace geomlib;

TEST_CASE("QuadArea") {
    const GeometryFactory* factory = GeometryFactory::getDefaultInstance();

    Point p1(0, 0, 0);
    Point p2(0, 1, 0);
    Point p3(1, 1, 0);
    Point p4(1, 0, 0);
    Point p5(0.5, 0.5, 0);
    Point p6(0, 1.5, 0);
    Point p7(1, 1.5, 0);

    KeyPoint* kp1 = factory->createKeypoint(p1);
    KeyPoint* kp2 = factory->createKeypoint(p2);
    KeyPoint* kp3 = factory->createKeypoint(p3);
    KeyPoint* kp4 = factory->createKeypoint(p4);
    KeyPoint* kp5 = factory->createKeypoint(p5);
    KeyPoint* kp6 = factory->createKeypoint(p6);
    KeyPoint* kp7 = factory->createKeypoint(p7);

    StraightLine* line1 = factory->createStraightLine(kp1, kp2);
    StraightLine* line2 = factory->createStraightLine(kp2, kp3);
    StraightLine* line3 = factory->createStraightLine(kp3, kp4);
    StraightLine* line4 = factory->createStraightLine(kp4, kp1);

    StraightLine* line5 = factory->createStraightLine(kp3, kp5);
    StraightLine* line6 = factory->createStraightLine(kp5, kp4);

    StraightLine* line7 = factory->createStraightLine(kp6, kp7);

    SECTION("Valid area") {
        vector<Line*> lines { line1, line2, line3, line4 };
        QuadArea area(lines);

        REQUIRE( area.lines.size() == 4 );
        REQUIRE( area.lines[0].direction == LineDirection::DIRECT );
        REQUIRE( area.lines[1].direction == LineDirection::DIRECT );
        REQUIRE( area.lines[2].direction == LineDirection::INVERSE );
        REQUIRE( area.lines[3].direction == LineDirection::INVERSE );
    }

    SECTION("Not Quadrilateral area") {
        vector<Line*> lines { line1, line2, line5, line6, line4 };
        REQUIRE_THROWS( QuadArea{lines} );
    }

    SECTION("Out of order lines") {
        vector<Line*> lines { line1, line3, line2, line4 };
        REQUIRE_THROWS( QuadArea{lines} );
    }

    SECTION("Not touching lines") {
        vector<Line*> lines { line1, line2, line7, line4 };
        REQUIRE_THROWS( QuadArea{lines} );
    }
}