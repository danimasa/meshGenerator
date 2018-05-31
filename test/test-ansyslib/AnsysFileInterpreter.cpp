#include "catch.hpp"

#include "AnsysFileInterpreter.hpp"

TEST_CASE("Should interprete KeyPoint", "[interpreter]") {
    AnsysFileInterpreter interpreter;

    SECTION("Should be null if empty line") {
        auto nullKp = interpreter.interpreteKeypoint("");
        REQUIRE( nullKp.get() == nullptr );
    }
    
    SECTION("Should be null if invalid line") {
        auto invalidKp = interpreter.interpreteKeypoint("asjkashljasdlkfjsh");
        REQUIRE( invalidKp.get() == nullptr );
    }

    SECTION("Should parse a simple line") {
        std::string line { "        1.    0.0000000000E+00    0.0000000000E+00    0.2625000000E+01" };
        auto validKp = interpreter.interpreteKeypoint(line);
        REQUIRE( validKp.get() != nullptr );
        REQUIRE( validKp.get()->x == 0 );
        REQUIRE( validKp.get()->y == 0 );
        REQUIRE( validKp.get()->z == 2.625 );
        REQUIRE (validKp.get()->id == 1 );
    }
}