#include "catch.hpp"

#include "KeypointInterpreter.hpp"
#include "GeometryFactory.hpp"

using namespace ansyslib;
using namespace geomlib;

TEST_CASE("KeypointInterpreter", "[interpreter]") {
    KeypointInterpreter interpreter;

    REQUIRE( interpreter.getBlockCode() == "10." );
    REQUIRE( interpreter.getLinesPerObject() == 1 );

    std::string content = "        1.    0.0000000000E+00    0.0000000000E+00    0.2625000000E+01";
    auto geom = interpreter.interpret(content);

    auto isKeypoint = dynamic_cast<KeyPoint*>(geom);
    REQUIRE( isKeypoint != NULL );
    REQUIRE( isKeypoint->x == 0 );
    REQUIRE( isKeypoint->y == 0 );
    REQUIRE( isKeypoint->z == 2.625 );
}