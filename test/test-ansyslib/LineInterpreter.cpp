#include "catch.hpp"

#include "LineInterpreter.hpp"
#include "GeometryList.hpp"
#include "GeometryFactory.hpp"

using namespace ansyslib;
using namespace geomlib;

TEST_CASE("LineInterpreter", "[interpreter]") {
    GeometryList list;
    LineInterpreter interpreter(&list);

    REQUIRE( interpreter.getBlockCode() == "20." );
    REQUIRE( interpreter.getLinesPerObject() == 4 );

    auto *factory = GeometryFactory::getDefaultInstance();

    auto p2 = Point(); // x = 0, y = 0, z = 0
    auto kp2 = factory->createKeypoint(p2);
    kp2->setID(2); // TODO: Deve ser gerado na construção do objeto
    list.add(kp2);

    auto p15 = Point(0.1072081295, 0, 0);
    auto kp15 = factory->createKeypoint(p15);
    kp15->setID(15);
    list.add(kp15);

    SECTION("StraightLine") {
        std::string content = "        1.        2.       15.\n"
            "        0.1072081295E+00    0.3661238196E+00    0.5341674540E-01    0.2625000000E+01\n"
            "        -0.4871291193E-16    0.1000000000E+01   -0.2598021970E-15\n"
            "        -0.2857142857E+00    0.9583148475E+00    0.2598021970E-15\n";

        auto geom = interpreter.interpret(content);

        auto isLine = dynamic_cast<Line*>(geom);
        REQUIRE( isLine != NULL );
        REQUIRE( isLine->getGeometryType() == GeometryType::Line);
        REQUIRE( isLine->getLineType() == LineType::StraightLine );
        REQUIRE( isLine->init_point->getID() == 2 );
        REQUIRE( isLine->final_point->getID() == 15 );
    }
    
    SECTION("ArcLine") {
        auto p23 = Point(0.3421000238, 0.1409523810, 2.625);
        auto kp23 = factory->createKeypoint(p23);
        kp23->setID(23); // TODO: Deve ser gerado na construção do objeto
        list.add(kp23);

        auto p31 = Point(0, 0.37, 2.625);
        auto kp31 = factory->createKeypoint(p31);
        kp31->setID(31);
        list.add(kp31);

        std::string content = "        3.       23.       31.\n"
            "        0.4365889727E+00    0.2058489969E+00    0.3074511188E+00    0.2625000000E+01\n"
            "        -0.3809523810E+00    0.9245946590E+00    0.2696692822E-15\n"
            "        -0.1000000000E+01   -0.4719212438E-15   -0.2696692822E-15\n";
        
        auto geom = interpreter.interpret(content);

        auto isLine = dynamic_cast<Line*>(geom);
        REQUIRE( isLine != NULL );
        REQUIRE( isLine->getGeometryType() == GeometryType::Line );
        REQUIRE( isLine->getLineType() == LineType::ArcLine );
        REQUIRE( isLine->init_point->getID() == 23 );
        REQUIRE( isLine->final_point->getID() == 31 );
    }

    SECTION("UnspecifiedLine") {
        std::string content = "        1.        2.       15.\n"
            "        0.2072081295E+00    0.3661238196E+00    0.5341674540E-01    0.2625000000E+01\n"
            "        -0.4871291193E-16    0.1000000000E+01   -0.2598021970E-15\n"
            "        -0.2857142857E+00    0.9583148475E+00    0.2598021970E-15\n";

        auto geom = interpreter.interpret(content);

        auto isLine = dynamic_cast<Line*>(geom);
        REQUIRE( isLine != NULL );
        REQUIRE( isLine->getGeometryType() == GeometryType::Line );
        REQUIRE( isLine->getLineType() == LineType::UnspecifiedLine );
        REQUIRE( isLine->init_point->getID() == 2 );
        REQUIRE( isLine->final_point->getID() == 15 );
    }
}
