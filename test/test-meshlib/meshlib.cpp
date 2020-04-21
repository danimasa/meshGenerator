#include <catch.hpp>
#include <vector>

#include "meshlib.hpp"
#include "GeometryFactory.hpp"

using namespace meshlib;
using namespace geomlib;

TEST_CASE("meshlib") {
    Mesh* regMesh = generateRegGrid(5, 2);

    REQUIRE( regMesh->getVertices().size() == 18 );
    REQUIRE( regMesh->getElements().size() == 10 );

    SECTION("Transfinite mapping") {
        auto factory = GeometryFactory::getDefaultInstance();

        auto p1 = Point(0, 0, 0);
        auto p2 = Point(5, 2, 3);
        auto p3 = Point(4, 6, 2);
        auto p4 = Point(1, 5, 1);

        auto kp1 = factory->createKeypoint(p1);
        auto kp2 = factory->createKeypoint(p2);
        auto kp3 = factory->createKeypoint(p3);
        auto kp4 = factory->createKeypoint(p4);

        auto l1 = factory->createStraightLine(kp1, kp2);
        auto l2 = factory->createStraightLine(kp2, kp3);
        auto l3 = factory->createStraightLine(kp3, kp4);
        auto l4 = factory->createStraightLine(kp4, kp1);

        std::vector<Line*> lines;
        lines.push_back(l1);
        lines.push_back(l2);
        lines.push_back(l3);
        lines.push_back(l4);

        QuadArea area(lines);

        auto transfMesh = transfiniteMapping(*regMesh, area);

        REQUIRE( transfMesh->getVertices().size() == 18 );
        REQUIRE( transfMesh->getElements().size() == 10 );
    }
}