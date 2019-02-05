#include "catch.hpp"

#include "VolumeInterpreter.hpp"
#include "GeometryList.hpp"
#include "GeometryFactory.hpp"

using namespace ansyslib;
using namespace geomlib;

TEST_CASE("VolumeInterpreter", "[interpreter]") {
    GeometryList list;
    VolumeInterpreter interpreter(&list);

    REQUIRE( interpreter.getBlockCode() == "40." );

    SECTION("Lines per Object") {
        std::string firstLine = "        1.        6.        0.";
        REQUIRE( interpreter.getLinesPerObject(firstLine) == 9 );
    }

    auto *factory = GeometryFactory::getDefaultInstance();

}