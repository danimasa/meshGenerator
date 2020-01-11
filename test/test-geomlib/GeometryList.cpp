#include <catch.hpp>

#include <memory>
#include "GeometryList.hpp"
#include "GeometryFactory.hpp"
#include "KeyPoint.hpp"

using namespace geomlib;

TEST_CASE("GeometryList.hpp") {
    GeometryList list;
    auto factory = GeometryFactory::getDefaultInstance();

    REQUIRE( list.size() == 0 );
    REQUIRE( list.capacity() == 0 );

    SECTION("Should reserve size") {
        list.reserveSize(10);

        REQUIRE(list.size() == 0 );
        REQUIRE(list.capacity() == 10);
    }

    SECTION("Should add a Geometry") {
        Point p;
        auto geom = factory->createKeypoint(p);
        geom->setID(1);
        list.add(geom);
        
        REQUIRE(list.size() == 1);
    }

    SECTION("Should get By Id") {
        Point p;
        auto geom = factory->createKeypoint(p);
        geom->setID(2);
        list.add(geom);

        auto kp = list.getByID(GeometryType::Keypoint, 2);
        REQUIRE( kp != NULL );
        REQUIRE( kp->getGeometryType() == GeometryType::Keypoint );
        REQUIRE( kp->getID() == 2 );
    }

    SECTION("Should substite if added the same ID") {
        Point p1;
        auto geom = factory->createKeypoint(p1);
        geom->setID(2);
        list.add(geom);

        Point p2(1, 1, 1);
        auto geom2 = factory->createKeypoint(p2);
        geom2->setID(2);
        list.add(geom2);

        auto kp = list.getByID(GeometryType::Keypoint, 2);
        Point* point = dynamic_cast<Point*>(kp);
        REQUIRE( *point == p2 );
    }
}