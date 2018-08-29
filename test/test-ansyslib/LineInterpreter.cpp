#include "catch.hpp"

#include "LineInterpreter.hpp"
#include "GeometryList.hpp"
#include "GeometryFactory.hpp"

using namespace ansyslib;
using namespace geomlib;

TEST_CASE("LineInterpreter Basic Props", "[interpreter]") {
    LineInterpreter interpreter(nullptr);

    REQUIRE( interpreter.getBlockCode() == "20." );
    REQUIRE( interpreter.getLinesPerObject() == 4 );
}

TEST_CASE("LineInterpreter StraightLine", "[interpreter]") {
    GeometryList list;
    auto *factory = GeometryFactory::getDefaultInstance();

    auto p2 = Point(); // x = 0, y = 0, z = 0
    auto kp2 = factory->createKeypoint(p2);
    kp2->setID(2); // TODO: Deve ser gerado na construção do objeto
    list.add(kp2);

    auto p15 = Point(0.1072081295, 0, 0);
    auto kp15 = factory->createKeypoint(p15);
    kp15->setID(15);
    list.add(kp15);

    LineInterpreter interpreter(&list);

    std::string content = "        1.        2.       15.\n"
        "        0.1072081295E+00    0.3661238196E+00    0.5341674540E-01    0.2625000000E+01\n"
        "        -0.4871291193E-16    0.1000000000E+01   -0.2598021970E-15\n"
        "        -0.2857142857E+00    0.9583148475E+00    0.2598021970E-15\n";

    auto geom = interpreter.interpret(content);

    auto isLine = dynamic_cast<Line*>(geom);
    REQUIRE( isLine != NULL );
    REQUIRE( isLine->getGeometryType() == "line");
    REQUIRE( isLine->getLineType() == "straight-line" );
    REQUIRE( isLine->init_point->getID() == 2 );
    REQUIRE( isLine->final_point->getID() == 15 );
}