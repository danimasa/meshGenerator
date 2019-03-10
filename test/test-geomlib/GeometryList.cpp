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
        geom->setID(1);
        list.add(geom.get());
        
        REQUIRE(list.size() == 1);
    }

    SECTION("Should get By Id") {
        std::unique_ptr<Geometry> geom { new KeyPoint() };
        geom->setID(2);
        list.add(geom.get());

        auto kp = list.getByID("keypoint", 2);
        REQUIRE( kp != NULL );
        REQUIRE( kp->getGeometryType() == "keypoint" );
        REQUIRE( kp->getID() == 2 );
    }

    SECTION("Should substite if added the same ID") {
        std::unique_ptr<Geometry> geom { new KeyPoint() };
        geom->setID(2);
        list.add(geom.get());

        Point p(1, 1, 1);
        std::unique_ptr<Geometry> geom2 { new KeyPoint(p) };
        geom2->setID(2);
        list.add(geom2.get());

        auto kp = list.getByID("keypoint", 2);
        Point* point = dynamic_cast<Point*>(kp);
        REQUIRE( *point == p );
    }
}