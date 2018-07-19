#include "catch.hpp"

#include "AnsysFileInterpreter.hpp"

TEST_CASE("Should interprete KeyPoint", "[interpreter]") {
    AnsysFileInterpreter interpreter;

    SECTION("Should be null if empty line") {
        auto nullKp = interpreter.interpreteKeypoint("");
        REQUIRE( nullKp == nullptr );
        delete nullKp;
    }
    
    SECTION("Should be null if invalid line") {
        auto invalidKp = interpreter.interpreteKeypoint("asjkashljasdlkfjsh");
        REQUIRE( invalidKp == nullptr );
        delete invalidKp;
    }

    SECTION("Should parse a simple line") {
        std::string line { "        1.    0.0000000000E+00    0.0000000000E+00    0.2625000000E+01" };
        auto validKp = interpreter.interpreteKeypoint(line);
        REQUIRE( validKp != nullptr );
        REQUIRE( validKp->x == 0 );
        REQUIRE( validKp->y == 0 );
        REQUIRE( validKp->z == 2.625 );
        REQUIRE (validKp->id == 1 );
        delete validKp;
    }
}

TEST_CASE("Should interprete Lines", "[interpreter]") {
    AnsysFileInterpreter interpreter;

    SECTION("Should be null if empty line") {
        auto nullLine = interpreter.interpreteLine("");
        REQUIRE( nullLine == nullptr );
        delete nullLine;
    }

    SECTION("Should be null if invalid line") {
        auto invalidLine = interpreter.interpreteLine("idhfalksdjhfalksjdh");
        REQUIRE( invalidLine == nullptr );
        delete invalidLine;
    }

    SECTION("Should parse a line") {
        std::string line =
            "        1.        2.       15.\n"
            "    0.1072081295E+00    0.3661238196E+00    0.5341674540E-01    0.2625000000E+01\n"
            "   -0.4871291193E-16    0.1000000000E+01   -0.2598021970E-15\n"
            "   -0.2857142857E+00    0.9583148475E+00    0.2598021970E-15\n";
        
    }
}