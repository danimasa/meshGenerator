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
    Area* area = factory->createArea(lines, line1, line4);

    REQUIRE( area != nullptr );
  }

  SECTION("Not permited non touching lines") {
    vector<Line*> lines { line1, line3, line2, line4 };
    REQUIRE_THROWS( factory->createArea(lines, line1, line4) );
  }

  SECTION("Not permited initial or final lines not in the list") {
    vector<Line*> lines { line1, line2, line3, line4 };
    REQUIRE_THROWS( factory->createArea(lines, line1, line5) );
    REQUIRE_THROWS( factory->createArea(lines, line5, line4) );
  }
}