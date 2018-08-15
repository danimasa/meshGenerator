#include "catch.hpp"

#include "LineInterpreter.hpp"
#include "Line.hpp"

using namespace ansyslib;
using namespace geomlib;

TEST_CASE("LineInterpreter", "[interpreter]") {
    LineInterpreter interpreter;

    REQUIRE( interpreter.getBlockCode() == "20." );
    REQUIRE( interpreter.getLinesPerObject() == 4 );

    std::string content = "        1.        2.       15.\n"
        "        0.1072081295E+00    0.3661238196E+00    0.5341674540E-01    0.2625000000E+01\n"
        "        -0.4871291193E-16    0.1000000000E+01   -0.2598021970E-15\n"
        "        -0.2857142857E+00    0.9583148475E+00    0.2598021970E-15\n";

    auto geom = interpreter.interpret(content);

    auto isLine = dynamic_cast<Line*>(geom);
    REQUIRE( isLine != NULL );
    REQUIRE( isLine->init_point == 2 );
    REQUIRE( isLine->final_point == 15 );
}