#include <catch.hpp>

#include "Area.hpp"
#include "GeometryFactory.hpp"
#include "StraightLine.hpp"

using namespace geomlib;

TEST_CASE("Area") {
  const GeometryFactory* factory = GeometryFactory::getDefaultInstance();

  Point p1(0, 0, 0);
  Point p2(0, 1, 0);
  Point p3(1, 1, 0);
  Point p4(1, 0, 0);

  KeyPoint* kp1 = factory->createKeypoint(p1);
  KeyPoint* kp2 = factory->createKeypoint(p2);
  KeyPoint* kp3 = factory->createKeypoint(p3);
  KeyPoint* kp4 = factory->createKeypoint(p4);

  StraightLine* line1 = factory->createStraightLine(kp1, kp2);
  StraightLine* line2 = factory->createStraightLine(kp2, kp3);
  StraightLine* line3 = factory->createStraightLine(kp3, kp4);
  StraightLine* line4 = factory->createStraightLine(kp4, kp1);
  StraightLine* line5 = factory->createStraightLine(kp2, kp4);

  SECTION("Valid area") {
    vector<Line*> lines { line1, line2, line3, line4 };
    Area::Loop loop(lines);
    vector<Area::Loop*> loops;
    loops.push_back(&loop);
    Area* area = factory->createArea(loops);

    REQUIRE( area != nullptr );
  }

  SECTION("Not permited non touching lines") {
    vector<Line*> lines { line1, line3, line4 };
    Area::Loop loop(lines);
    vector<Area::Loop*> loops;
    loops.push_back(&loop);
    REQUIRE_THROWS( factory->createArea(loops) );
  }

  SECTION("Direct and Inverse Lines in Loop") {
    StraightLine* line6 = factory->createStraightLine(kp4, kp3);

    vector<Line*> lines { line1, line2, line6, line4 };
    Area::Loop loop(lines);

    REQUIRE( loop.lines[0].direction == LineDirection::DIRECT );
    REQUIRE( loop.lines[1].direction == LineDirection::DIRECT );
    REQUIRE( loop.lines[2].direction == LineDirection::INVERSE );
    REQUIRE( loop.lines[3].direction == LineDirection::DIRECT );
  }
}