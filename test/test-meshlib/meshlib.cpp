#include <catch.hpp>

#include "meshlib.hpp"

using namespace meshlib;

TEST_CASE("meshlib") {
    Mesh regMesh = generateRegGrid(5, 2);

    REQUIRE( regMesh.vertices.size() == 18 );
    REQUIRE( regMesh.elements.size() == 10 );
}