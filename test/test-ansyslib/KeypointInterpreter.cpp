#include "catch.hpp"

#include "KeypointInterpreter.hpp"
#include "GeometryFactory.hpp"

using namespace ansyslib;
using namespace geomlib;

TEST_CASE("KeypointInterpreter", "[interpreter]") {
    KeypointInterpreter interpreter;

    REQUIRE( interpreter.getBlockCode() == "10." );
    SECTION("one line per object") {
        std::string content = "line";
        REQUIRE(interpreter.accumulateLine(content) == false);
        REQUIRE(interpreter.accumulateLine(content) == true);
    }

    std::string content = "        1.    0.0000000000E+00    0.0000000000E+00    0.2625000000E+01";
    interpreter.setAccumulatedLines(content);
    auto geom = interpreter.interpret();

    auto isKeypoint = dynamic_cast<KeyPoint*>(geom);
    REQUIRE( isKeypoint != NULL );
    REQUIRE( isKeypoint->x == 0 );
    REQUIRE( isKeypoint->y == 0 );
    REQUIRE( isKeypoint->z == 2.625 );
}