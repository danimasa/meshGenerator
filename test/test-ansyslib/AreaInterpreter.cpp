#include "catch.hpp"

#include "AreaInterpreter.hpp"
#include "GeometryList.hpp"
#include "GeometryFactory.hpp"

using namespace ansyslib;
using namespace geomlib;

TEST_CASE("AreaInterpreter", "[interpreter]") {
  GeometryList list;
  AreaInterpreter interpreter(&list);

  REQUIRE( interpreter.getBlockCode() == "30." );

  SECTION("Lines per Object") {
    std::string firstLine = "        1.        4.        0.";
    REQUIRE( interpreter.getLinesPerObject(firstLine) == 7 );
  }
}