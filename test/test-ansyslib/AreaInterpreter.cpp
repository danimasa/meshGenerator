#include "catch.hpp"

#include <sstream>

#include "AreaInterpreter.hpp"
#include "GeometryList.hpp"
#include "GeometryFactory.hpp"
#include "LineInterpreter.hpp"

using namespace ansyslib;
using namespace geomlib;

using Catch::Matchers::Contains;

TEST_CASE("AreaInterpreter", "[interpreter]") {
  GeometryList list;
  AreaInterpreter interpreter(&list);

  REQUIRE( interpreter.getBlockCode() == "30." );

  auto *factory = GeometryFactory::getDefaultInstance();

  SECTION("Common List") {
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

    // Inner loop
    auto p5 = Point(0.25, 0.25, 0);
    auto kp5 = factory->createKeypoint(p5);
    kp5->setID(5);
    list.add(kp5);

    auto p6 = Point(0.75, 0.25, 0);
    auto kp6 = factory->createKeypoint(p6);
    kp6->setID(6);
    list.add(kp6);

    auto p7 = Point(0.75, 0.75, 0);
    auto kp7 = factory->createKeypoint(p7);
    kp7->setID(7);
    list.add(kp7);

    auto p8 = Point(0.25, 0.75, 0);
    auto kp8 = factory->createKeypoint(p8);
    kp8->setID(8);
    list.add(kp8);

    // Outer loop
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

    // Inner loop
    auto line2 = factory->createStraightLine(kp5, kp6);
    line2->setID(2);
    list.add(line2);

    auto line3 = factory->createStraightLine(kp6, kp7);
    line3->setID(3);
    list.add(line3);

    auto line5 = factory->createStraightLine(kp7, kp8);
    line5->setID(5);
    list.add(line5);

    auto line6 = factory->createStraightLine(kp8, kp5);
    line6->setID(6);
    list.add(line6);

    SECTION("Valid Area") {
      std::string content = "        1.        4.        0.\n"
          " L        1.\n"
          "1.\n"
          "46.\n"
          "4.\n"
          "16.\n";
      
      interpreter.setAccumulatedLines(content);
      auto geom = interpreter.interpret();

      auto isArea = dynamic_cast<Area*>(geom);
      REQUIRE( isArea != NULL );
      REQUIRE( isArea->getGeometryType() == GeometryType::Area );
      REQUIRE( isArea->loops.size() == 1);
      REQUIRE( isArea->loops[0]->lines.size() == 4);
    }

    SECTION("Read inner loop") {
      std::string content = "        1.        4.        0.\n"
          " L        1.\n"
          "1.\n"
          "46.\n"
          "4.\n"
          "16.\n"
          " L        2.\n"
          "2.\n"
          "3.\n"
          "5.\n"
          "6.\n";
      
      interpreter.setAccumulatedLines(content);
      auto geom = interpreter.interpret();

      auto isArea = dynamic_cast<Area*>(geom);
      REQUIRE( isArea != NULL );
      REQUIRE( isArea->getGeometryType() == GeometryType::Area );
      REQUIRE( isArea->loops.size() == 2 );
      REQUIRE( isArea->loops[0]->lines.size() == 4 );
      REQUIRE( isArea->loops[1]->lines.size() == 4 );
      REQUIRE( isArea->loops[0]->lines[0]->getID() == 1 );
      REQUIRE( isArea->loops[1]->lines[0]->getID() == 2 );
    }

    SECTION("Empty block returns null") {
      std::string content = "";
      
      interpreter.setAccumulatedLines(content);
      auto geom = interpreter.interpret();

      auto area = dynamic_cast<Area*>(geom);
      REQUIRE( area == NULL );
    }

    SECTION("Minimal of 3 lines to define an area") {
      std::string content = "        1.        2.        0.\n"
          " L        1.\n"
          "1.\n"
          "4.\n";

      interpreter.setAccumulatedLines(content);
      REQUIRE_THROWS_WITH( interpreter.interpret(), Contains("minimal of 3 lines") );
    }

    SECTION("Line not in list of lines") {
      std::string content = "        1.        4.        0.\n"
        " L        1.\n"
        "1.\n"
        "4.\n"
        "10.\n"
        "46.\n";

      interpreter.setAccumulatedLines(content);
      REQUIRE_THROWS_WITH( interpreter.interpret(), Contains("not in the list of lines") );
    }
  }
  
  SECTION("Discover concatenated lines") {
    auto p5 = Point(0, 0, 0);
    auto kp5 = factory->createKeypoint(p5);
    kp5->setID(5);
    list.add(kp5);

    auto p6 = Point(1, 0, 0);
    auto kp6 = factory->createKeypoint(p6);
    kp6->setID(6);
    list.add(kp6);

    auto p7 = Point(1, 1, 0);
    auto kp7 = factory->createKeypoint(p7);
    kp7->setID(7);
    list.add(kp7);

    auto p8 = Point(0.5, 1.3, 0);
    auto kp8 = factory->createKeypoint(p8);
    kp8->setID(8);
    list.add(kp8);

    auto p9 = Point(0, 1, 0);
    auto kp9 = factory->createKeypoint(p9);
    kp9->setID(9);
    list.add(kp9);

    LineInterpreter lineInterpreter(&list);
    std::string line2Content = "        2.        5.       6.       1.0000000000E+00\n"
        "        0.5000000000E+00    0.0000000000E+00    0.0000000000E+00\n"
        "        1.0000000000E+00    0.0000000000E+00    0.0000000000E+00\n"
        "        1.0000000000E+00    0.0000000000E+00    0.0000000000E+00\n";
    lineInterpreter.setAccumulatedLines(line2Content);
    auto line2 = lineInterpreter.interpret();
    list.add(line2);

    std::string line3Content = "        3.        6.       7.       1.0000000000E+00\n"
        "        1.0000000000E+00    0.5000000000E+00    0.0000000000E+00\n"
        "        0.0000000000E+00    1.0000000000E+00    0.0000000000E+00\n"
        "        0.0000000000E+00    1.0000000000E+00    0.0000000000E+00\n";
    lineInterpreter.setAccumulatedLines(line3Content);
    auto line3 = lineInterpreter.interpret();
    list.add(line3);

    std::string line5Content = "        5.        7.       9.       1.1661903790E+00\n"
        "        0.5000000000E+00    1.3000000000E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    0.5144957554E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    -0.5144957554E+00    0.0000000000E+00\n";
    lineInterpreter.setAccumulatedLines(line5Content);
    auto line5 = lineInterpreter.interpret();
    list.add(line5);

    std::string line8Content = "        8.        9.       5.       1.0000000000E+00\n"
        "        0.0000000000E+00    0.5000000000E+00    0.0000000000E+00\n"
        "        0.0000000000E+00    -1.0000000000E+00    0.0000000000E+00\n"
        "        0.0000000000E+00    -1.0000000000E+00    0.0000000000E+00\n";
    lineInterpreter.setAccumulatedLines(line8Content);
    auto line8 = lineInterpreter.interpret();
    list.add(line8);

    // Lines of concatenated
    std::string line9Content = "        9.        7.       8.       0.5830951895E+00\n"
        "        0.7500000000E+00    1.1500000000E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    0.5144957554E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    0.5144957554E+00    0.0000000000E+00\n";
    lineInterpreter.setAccumulatedLines(line9Content);
    auto line9 = lineInterpreter.interpret();
    list.add(line9);

    std::string line10Content = "        10.        8.       9.       0.5830951895E+00\n"
        "        0.2500000000E+00    1.1500000000E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    -0.5144957554E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    -0.5144957554E+00    0.0000000000E+00\n";
    lineInterpreter.setAccumulatedLines(line10Content);
    auto line10 = lineInterpreter.interpret();
    list.add(line10);

    auto undefinedLine5 = dynamic_cast<Line*>(line5);
    REQUIRE( undefinedLine5->getLineType() == LineType::UnspecifiedLine );

    std::string content = "        2.       6.       1.\n"
      " L        1.\n"
      "2.\n"
      "3.\n"
      "5.\n"
      "8.\n"
      "5.\n";

    interpreter.setAccumulatedLines(content);
    auto geom = interpreter.interpret();
    auto isArea = dynamic_cast<Area*>(geom);

    REQUIRE( isArea->getGeometryType() == GeometryType::Area );
    REQUIRE( isArea->loops[0]->lines.size() == 4);

    auto concatenatedLine5 = dynamic_cast<Line*>(list.getByID(GeometryType::Line, 5));
    // create polyline type
    REQUIRE( concatenatedLine5->getLineType() == LineType::Polyline );
    // validate lines inner polyline
    Polyline* polyline = dynamic_cast<Polyline*>(concatenatedLine5);
    auto lines = polyline->get_lines();
    int size = lines.size();
    Line* l1 = lines[0];
    REQUIRE( lines[0]->getID() == 9 );
    REQUIRE( lines[1]->getID() == 10 );
  }
}