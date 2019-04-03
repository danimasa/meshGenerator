#include <catch.hpp>
#include <vector>

#include "LineAnalysis.hpp"
#include "GeometryFactory.hpp"

using namespace processlib;
using namespace geomlib;
using namespace std;

TEST_CASE("LineAnalysis.hpp") {
    GeometryList list;
    auto factory = GeometryFactory::getDefaultInstance();

    SECTION("Caso 1") {
        auto p1 = Point(0, 0, 0);
        auto p2 = Point(2, 0, 0);
        auto p3 = Point(2, 1, 0);
        auto p4 = Point(0, 1, 0);
        auto p6 = Point(0.8, 0, 0);
        auto p7 = Point(0.4, 1, 0);

        auto kp1 = factory->createKeypoint(p1);
        auto kp2 = factory->createKeypoint(p2);
        auto kp3 = factory->createKeypoint(p3);
        auto kp4 = factory->createKeypoint(p4);

        auto kp5 = factory->createKeypoint(p6);
        auto kp6 = factory->createKeypoint(p7);

        list.add(kp1);
        list.add(kp2);
        list.add(kp3);
        list.add(kp4);
        list.add(kp5);
        list.add(kp6);

        auto l1 = factory->createStraightLine(kp1, kp2);
        auto l2 = factory->createStraightLine(kp2, kp3);
        auto l3 = factory->createStraightLine(kp3, kp4);
        auto l4 = factory->createStraightLine(kp4, kp1);
        auto l5 = factory->createStraightLine(kp5, kp6);

        list.add(l1);
        list.add(l2);
        list.add(l3);
        list.add(l4);
        list.add(l5);

        vector<Line*> lines {l1, l2, l3, l4};
        auto a1 = factory->createArea(lines, l1, l4);
        list.add(a1);

        LineAnalysis analyser(&list);
        analyser.findSingularities();

        auto rawArea = list.getByID(GeometryType::Area, a1->getID());
        Area* afterArea = dynamic_cast<Area*>(rawArea);

        auto lafter1 = afterArea->lines[0];
        auto lafter2 = afterArea->lines[2];
        REQUIRE( lafter1->getLineType() == LineType::Polyline );
        REQUIRE( lafter2->getLineType() == LineType::Polyline );

        Polyline *poly1 = dynamic_cast<Polyline*>(lafter1);
        REQUIRE( poly1->get_lines()[0]->init_point == kp1 );
        REQUIRE( poly1->get_lines()[0]->final_point == kp5 );
        REQUIRE( poly1->get_lines()[1]->init_point == kp5 );
        REQUIRE( poly1->get_lines()[1]->final_point == kp2 );

        Polyline *poly2 = dynamic_cast<Polyline*>(lafter2);
        REQUIRE( poly2->get_lines()[0]->init_point == kp3 );
        REQUIRE( poly2->get_lines()[0]->final_point == kp6 );
        REQUIRE( poly2->get_lines()[1]->init_point == kp6 );
        REQUIRE( poly2->get_lines()[1]->final_point == kp4 );
    }

}