#include <catch.hpp>

#include "AreaAnalysis.hpp"
#include "GeometryFactory.hpp"
#include "GeometryList.hpp"

using namespace processlib;
using namespace geomlib;

TEST_CASE("AreaAnalysis.hpp") {
    auto factory = GeometryFactory::getDefaultInstance();

    SECTION("Subdivide Triangle") {
        GeometryList list;
        auto p1 = Point(0, 0, 0);
        auto p2 = Point(1, 0, 0);
        auto p3 = Point(0.5, 1, 0);

        auto kp1 = factory->createKeypoint(p1);
        auto kp2 = factory->createKeypoint(p2);
        auto kp3 = factory->createKeypoint(p3);

        list.add(kp1);
        list.add(kp2);
        list.add(kp3);

        auto l1 = factory->createStraightLine(kp1, kp2);
        auto l2 = factory->createStraightLine(kp2, kp3);
        auto l3 = factory->createStraightLine(kp3, kp1);

        list.add(l1);
        list.add(l2);
        list.add(l3);

        vector<Line*> lines {l1, l2, l3};
        auto a1 = factory->createArea(lines);
        list.add(a1);

        AreaAnalysis analyser(&list);
        analyser.subdivideArea(a1);

        auto children = a1->getChildren();
        REQUIRE( children.size() == 3 );

        auto loop = a1->loops[0];
        REQUIRE( loop->lines[0]->getLineType() == LineType::Polyline );
        REQUIRE( loop->lines[1]->getLineType() == LineType::Polyline );
        REQUIRE( loop->lines[2]->getLineType() == LineType::Polyline );

        auto pLine1 = dynamic_cast<Polyline*>(loop->lines[0]);
        auto pLine2 = dynamic_cast<Polyline*>(loop->lines[1]);
        auto pLine3 = dynamic_cast<Polyline*>(loop->lines[2]);

        auto mid1 = pLine1->intermidiaryPoints()[0];
        auto mid2 = pLine2->intermidiaryPoints()[0];
        auto mid3 = pLine3->intermidiaryPoints()[0];

        REQUIRE( mid1->x == 0.5 );
        REQUIRE( mid1->y == 0.0 );
        REQUIRE( mid1->z == 0.0 );

        REQUIRE( mid2->x == 0.75 );
        REQUIRE( mid2->y == 0.5 );
        REQUIRE( mid2->z == 0.0 );

        REQUIRE( mid3->x == 0.25 );
        REQUIRE( mid3->y == 0.5 );
        REQUIRE( mid3->z == 0.0 );

        auto mid4 = children[0]->loops[0]->lines[1]->final_point;

        REQUIRE( mid4->x == 0.5 );
        REQUIRE( mid4->y == 0.30901699437494751 );
        REQUIRE( mid4->z == 0.0 );
    }
}   