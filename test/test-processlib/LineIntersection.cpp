#include <catch.hpp>
#include <vector>

#include "LineIntersection.hpp"
#include "GeometryFactory.hpp"

using namespace processlib;
using namespace geomlib;

TEST_CASE("LineIntersection.hpp") {
    auto factory = GeometryFactory::getDefaultInstance();

    SECTION("2D Intersection") {
        auto p1 = Point(0, 0, 0);
        auto p2 = Point(1, 1, 0);
        auto p3 = Point(0, 1, 0);
        auto p4 = Point(1, 0, 0);

        auto kp1 = factory->createKeypoint(p1);
        auto kp2 = factory->createKeypoint(p2);
        auto kp3 = factory->createKeypoint(p3);
        auto kp4 = factory->createKeypoint(p4);

        auto l1 = factory->createStraightLine(kp1, kp2);
        auto l2 = factory->createStraightLine(kp3, kp4);

        std::vector<Line*> lines;
        lines.push_back(l1);
        lines.push_back(l2);
        LineIntersection intersect_job(lines);

        auto intersections = intersect_job.findIntersections();
        
        REQUIRE( intersections.size() == 1 );
        auto inters = intersections[0];
        REQUIRE( inters.segments.size() == 2 );
        REQUIRE( inters.segments[0] == l2 );
        REQUIRE( inters.segments[1] == l1 );

        REQUIRE( inters.intersection.x == 0.5 );
        REQUIRE( inters.intersection.y == 0.5 );
        REQUIRE( inters.intersection.z == 0.0 );
    }

    SECTION("3D Intersection") {
        auto p1 = Point(0, 0, 0);
        auto p2 = Point(1, 1, 1);
        auto p3 = Point(0, 1, 0);
        auto p4 = Point(1, 0, 1);

        auto kp1 = factory->createKeypoint(p1);
        auto kp2 = factory->createKeypoint(p2);
        auto kp3 = factory->createKeypoint(p3);
        auto kp4 = factory->createKeypoint(p4);

        auto l1 = factory->createStraightLine(kp1, kp2);
        auto l2 = factory->createStraightLine(kp3, kp4);

        std::vector<Line*> lines;
        lines.push_back(l1);
        lines.push_back(l2);
        LineIntersection intersect_job(lines);

        auto intersections = intersect_job.findIntersections();
        
        REQUIRE( intersections.size() == 1 );
        auto inters = intersections[0];
        REQUIRE( inters.segments.size() == 2 );
        REQUIRE( inters.segments[0] == l2 );
        REQUIRE( inters.segments[1] == l1 );

        REQUIRE( inters.intersection.x == 0.5 );
        REQUIRE( inters.intersection.y == 0.5 );
        REQUIRE( inters.intersection.z == 0.5 );
    }

    SECTION("Not intersecting lines") {
        auto p1 = Point(0, 0, 0);
        auto p2 = Point(1, 1, 1);
        auto p3 = Point(0, 1, 0);
        auto p4 = Point(1, 0, 0.8);

        auto kp1 = factory->createKeypoint(p1);
        auto kp2 = factory->createKeypoint(p2);
        auto kp3 = factory->createKeypoint(p3);
        auto kp4 = factory->createKeypoint(p4);

        auto l1 = factory->createStraightLine(kp1, kp2);
        auto l2 = factory->createStraightLine(kp3, kp4);

        std::vector<Line*> lines;
        lines.push_back(l1);
        lines.push_back(l2);
        LineIntersection intersect_job(lines);

        auto intersections = intersect_job.findIntersections();
        
        REQUIRE( intersections.size() == 0 );
    }
}