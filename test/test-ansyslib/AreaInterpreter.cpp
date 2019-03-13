#include "catch.hpp"

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

  SECTION("Lines per Object") {
    std::string firstLine = "        1.        4.        0.";
    REQUIRE( interpreter.getLinesPerObject(firstLine) == 7 );
  }

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

    SECTION("Not in order Valid Area") {
      std::string content = "        1.        4.        0.\n"
          "1.\n"
          "16.\n"
          "4.\n"
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
    std::string line2Content = "        2.        5.       6.\n"
        "        1.0000000000E+00    0.5000000000E+00    0.0000000000E+00    0.0000000000E+00\n"
        "        1.0000000000E+00    0.0000000000E+00    0.0000000000E+00\n"
        "        1.0000000000E+00    0.0000000000E+00    0.0000000000E+00\n";
    auto line2 = lineInterpreter.interpret(line2Content);
    list.add(line2);

    std::string line3Content = "        3.        6.       7.\n"
        "        1.0000000000E+00    1.0000000000E+00    0.5000000000E+00    0.0000000000E+00\n"
        "        0.0000000000E+00    1.0000000000E+00    0.0000000000E+00\n"
        "        0.0000000000E+00    1.0000000000E+00    0.0000000000E+00\n";
    auto line3 = lineInterpreter.interpret(line3Content);
    list.add(line3);

    std::string line5Content = "        5.        7.       9.\n"
        "        1.1661903790E+00    0.5000000000E+00    1.3000000000E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    0.5144957554E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    -0.5144957554E+00    0.0000000000E+00\n";
    auto line5 = lineInterpreter.interpret(line5Content);
    list.add(line5);

    std::string line8Content = "        8.        9.       5.\n"
        "        1.0000000000E+00    0.0000000000E+00    0.5000000000E+00    0.0000000000E+00\n"
        "        0.0000000000E+00    -1.0000000000E+00    0.0000000000E+00\n"
        "        0.0000000000E+00    -1.0000000000E+00    0.0000000000E+00\n";
    auto line8 = lineInterpreter.interpret(line8Content);
    list.add(line8);

    // Lines of concatenated
    std::string line9Content = "        9.        7.       8.\n"
        "        0.5830951895E+00    0.7500000000E+00    1.1500000000E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    0.5144957554E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    0.5144957554E+00    0.0000000000E+00\n";
    auto line9 = lineInterpreter.interpret(line9Content);
    list.add(line9);

    std::string line10Content = "        10.        8.       9.\n"
        "        0.5830951895E+00    0.2500000000E+00    1.1500000000E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    -0.5144957554E+00    0.0000000000E+00\n"
        "        -0.8574929257E+00    -0.5144957554E+00    0.0000000000E+00\n";
    auto line10 = lineInterpreter.interpret(line10Content);
    list.add(line10);

    auto undefinedLine5 = dynamic_cast<Line*>(line5);
    REQUIRE( undefinedLine5->getLineType() == LineType::UnspecifiedLine );

    std::string content = "        2.       4.       1.\n"
      "2.\n"
      "3.\n"
      "5.\n"
      "8.\n"
      "5.\n"
      "2.\n"
      "3.\n";

    auto geom = interpreter.interpret(content);
    auto isArea = dynamic_cast<Area*>(geom);

    REQUIRE( isArea->getGeometryType() == "area" );
    REQUIRE( isArea->lines.size() == 4);
    REQUIRE( isArea->first_line->getID() == 2);
    REQUIRE( isArea->last_line->getID() == 3); // second_line not last_line

    auto concatenatedLine5 = dynamic_cast<Line*>(list.getByID("line", 5));
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