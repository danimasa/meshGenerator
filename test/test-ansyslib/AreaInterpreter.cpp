#include "catch.hpp"

#include "AreaInterpreter.hpp"
#include "GeometryList.hpp"
#include "GeometryFactory.hpp"

using namespace ansyslib;
using namespace geomlib;

using Catch::Matchers::Contains;

TEST_CASE("AreaInterpreter", "[interpreter]") {
  GeometryList list;
  AreaInterpreter interpreter(&list);

  REQUIRE( interpreter.getBlockCode() == "30." );

  SECTION("Lines per Object") {
    std::string firstLine = "        1.        4.        0.";
    REQUIRE( interpreter.getLinesPerObject(firstLine) == 7 );
  }

  auto *factory = GeometryFactory::getDefaultInstance();

  auto p1 = Point();
  auto kp1 = factory->createKeypoint(p1);
  kp1->setID(1);
  list.add(kp1);

  auto p2 = Point(0, 1, 0);
  auto kp2 = factory->createKeypoint(p2);
  kp2->setID(2);
  list.add(kp2);

  auto p3 = Point(1, 1, 0);
  auto kp3 = factory->createKeypoint(p3);
  kp3->setID(3);
  list.add(kp3);

  auto p4 = Point(1, 0, 0);
  auto kp4 = factory->createKeypoint(p4);
  kp4->setID(4);
  list.add(kp4);

  auto line1 = factory->createStraightLine(kp1, kp2);
  line1->setID(1);
  list.add(line1);

  auto line4 = factory->createStraightLine(kp2, kp3);
  line4->setID(4);
  list.add(line4);

  auto line16 = factory->createStraightLine(kp3, kp4);
  line16->setID(16);
  list.add(line16);

  auto line46 = factory->createStraightLine(kp4, kp1);
  line46->setID(46);
  list.add(line46);

  SECTION("Valid Area") {
    std::string content = "        1.        4.        0.\n"
        "1.\n"
        "4.\n"
        "16.\n"
        "46.\n"
        "1.\n"
        "46.\n";
    
    auto geom = interpreter.interpret(content);

    auto isArea = dynamic_cast<Area*>(geom);
    REQUIRE( isArea != NULL );
    REQUIRE( isArea->getGeometryType() == "area" );
    REQUIRE( isArea->lines.size() == 4);
    REQUIRE( isArea->first_line->getID() == 1);
    REQUIRE( isArea->last_line->getID() == 46);
  }

  SECTION("Empty block returns null") {
    std::string content = "";
    
    auto geom = interpreter.interpret(content);

    auto area = dynamic_cast<Area*>(geom);
    REQUIRE( area == NULL );
  }

  SECTION("Minimal of 3 lines to define an area") {
    std::string content = "        1.        2.        0.\n"
        "1.\n"
        "4.\n"
        "1.\n"
        "4.\n";

    REQUIRE_THROWS_WITH( interpreter.interpret(content), Contains("minimal of 3 lines") );
  }

  SECTION("Line not in list of lines") {
    std::string content = "        1.        4.        0.\n"
      "1.\n"
      "4.\n"
      "10.\n"
      "46.\n"
      "1.\n"
      "46.\n";

    REQUIRE_THROWS_WITH( interpreter.interpret(content), Contains("not in the list of lines") );
  }
}