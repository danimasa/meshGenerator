#include "AreaAnalysis.hpp"

#include "GeometryFactory.hpp"
#include "LineIntersection.hpp"

namespace processlib {

using namespace geomlib;

void AreaAnalysis::subdivideArea(Area* area) {
    if(area->loops.size() == 1 && area->loops[0]->lines.size() == 3) {
        subdivideTriangle(area);
    }
}

void AreaAnalysis::subdivideTriangle(Area* area) {
    auto factory = GeometryFactory::getDefaultInstance();

    auto l1 = area->loops[0]->lines[0];
    auto l2 = area->loops[0]->lines[1];
    auto l3 = area->loops[0]->lines[2];

    auto mid1 = l1->pointAtPosition(0.5);
    auto mid2 = l2->pointAtPosition(0.5);
    auto mid3 = l3->pointAtPosition(0.5);

    auto kp1 = factory->createKeypoint(mid1);
    auto kp2 = factory->createKeypoint(mid2);
    auto kp3 = factory->createKeypoint(mid3);

    geomList->add(kp1);
    geomList->add(kp2);
    geomList->add(kp3);

    auto pLine1 = brokeInPolyline(l1, kp1);
    auto pLine2 = brokeInPolyline(l2, kp2);
    auto pLine3 = brokeInPolyline(l3, kp3);

    area->loops[0]->lines[0] = pLine1;
    area->loops[0]->lines[1] = pLine2;
    area->loops[0]->lines[2] = pLine3;

    auto vLine1 = l1->get_init_vector();
    auto vLine2 = l1->init_point == l3->final_point
        ? l3->get_init_vector().invert()
        : l3->get_init_vector();

    auto bissect1 = Vector::bissector(vLine1, vLine2);

    auto vLine3 = l1->final_point == l2->final_point
        ? l2->get_init_vector().invert()
        : l2->get_init_vector();

    auto bissect2 = Vector::bissector(vLine1.invert(), vLine3);

    LineIntersection::ContinuousLine c1 { l1->init_point, &bissect1 };
    LineIntersection::ContinuousLine c2 { l1->final_point, &bissect2 };

    auto mid4 = LineIntersection::findIntersection(c1, c2);
    auto kp4 = factory->createKeypoint(mid4);
    geomList->add(kp4);

    auto nl1 = factory->createStraightLine(kp1, kp4);
    auto nl2 = factory->createStraightLine(kp2, kp4);
    auto nl3 = factory->createStraightLine(kp3, kp4);

    int l3Index = l1->init_point == l3->final_point ? 1 : 0;
    int l2Index = l1->final_point == l2->init_point ? 0 : 1;

    vector<Line*> a1Lines {
        pLine1->get_lines()[0],
        nl1,
        nl3,
        pLine3->get_lines()[l3Index]
    };

    vector<Line*> a2Lines {
        pLine1->get_lines()[1],
        pLine2->get_lines()[l2Index],
        nl2,
        nl1
    };

    vector<Line*> a3Lines {
        pLine2->get_lines()[l2Index == 0 ? 1 : 0],
        pLine3->get_lines()[l3Index == 0 ? 1 : 0],
        nl3,
        nl2
    };

    auto a1 = factory->createArea(a1Lines);
    auto a2 = factory->createArea(a2Lines);
    auto a3 = factory->createArea(a3Lines);

    area->addChild(a1);
    area->addChild(a2);
    area->addChild(a3);
}

}