#include <catch.hpp>

#include <memory>
#include "GeometryList.hpp"
#include "KeyPoint.hpp"

using namespace geomlib;

TEST_CASE("GeometryList.hpp") {
    GeometryList list;

    REQUIRE( list.size() == 0 );
    REQUIRE( list.capacity() == 0 );

    SECTION("Should reserve size") {
        list.reserveSize(10);

        REQUIRE(list.size() == 0 );
        REQUIRE(list.capacity() == 10);
    }

    SECTION("Should add a Geometry") {
        std::unique_ptr<Geometry> geom { new KeyPoint() };
        list.add(geom.get());
        
        REQUIRE(list.size() == 1);
    }
}