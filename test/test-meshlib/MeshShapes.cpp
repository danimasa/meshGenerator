#include <catch.hpp>

#include "MeshShapes.hpp"
#include "AreaMesh.hpp"
#include "GeometryFactory.hpp"

using namespace meshlib;
using namespace geomlib;

using RShape = MeshShapes::RelativeShapes;

TEST_CASE("MeshShapes.hpp") {
    auto factory = GeometryFactory::getDefaultInstance();

    auto p1 = Point(0, 0, 0);
    auto p2 = Point(2, -1, 0);
    auto p3 = Point(2, 2, 0);
    auto p4 = Point(0, 1, 0);

    auto kp1 = factory->createKeypoint(p1);
    auto kp2 = factory->createKeypoint(p2);
    auto kp3 = factory->createKeypoint(p3);
    auto kp4 = factory->createKeypoint(p4);

    auto l1 = factory->createStraightLine(kp1, kp2);
    auto l2 = factory->createStraightLine(kp2, kp3);
    auto l3 = factory->createStraightLine(kp3, kp4);
    auto l4 = factory->createStraightLine(kp4, kp1);

    std::vector<Line*> lines { l1, l2, l3, l4 };
    QuadArea a1(lines);

    SECTION("Throw error if without elements quantity") {
        REQUIRE_THROWS( MeshShapes::generateShapeList(a1) );
    }

    SECTION("Insufficient mesh size") {
        AreaMesh area(a1, 1);

        REQUIRE_THROWS_WITH( MeshShapes::generateShapeList(area), Catch::Contains("insuficient element size") );
    }

    SECTION("U Subdivision") {
        AreaMesh area(a1, 0.2);

        auto shapeList = MeshShapes::generateShapeList(area);

        REQUIRE( shapeList.size() == 6 );
        REQUIRE( shapeList[0] == RShape::PPOO_270 );
        REQUIRE( shapeList[1] == RShape::PPOO_270 );
        REQUIRE( shapeList[2] == RShape::PPOO_270 );
        REQUIRE( shapeList[3] == RShape::PPOO_270 );
        REQUIRE( shapeList[4] == RShape::PPOO_270 );
        REQUIRE( shapeList[5] == RShape::OOOO );
    }

    SECTION("L Subdivision") {
        auto p5 = Point(1, 0, 0);
        auto p6 = Point(1, 1, 0);
        auto p7 = Point(0, 1.25, 0);

        auto kp5 = factory->createKeypoint(p5);
        auto kp6 = factory->createKeypoint(p6);
        auto kp7 = factory->createKeypoint(p7);

        auto l5 = factory->createStraightLine(kp1, kp5);
        auto l6 = factory->createStraightLine(kp5, kp6);
        auto l7 = factory->createStraightLine(kp6, kp7);
        auto l8 = factory->createStraightLine(kp7, kp1);

        std::vector<Line*> lsubLines { l5, l6, l7, l8 };
        QuadArea a2(lsubLines);
        AreaMesh area(a2, 0.25);

        auto shapeList = MeshShapes::generateShapeList(area);

        REQUIRE( shapeList.size() == 2 );
        REQUIRE( shapeList[0] == RShape::POOO_90 );
        REQUIRE( shapeList[1] == RShape::OOOO );
    }

    SECTION("Mixed Subdivision") {
        a1.lines[0].qtdElements = 10;
        a1.lines[1].qtdElements = 11;
        a1.lines[2].qtdElements = 13;
        a1.lines[3].qtdElements = 10;
        
        AreaMesh area(a1, 0.25);
        auto shapeList = MeshShapes::generateShapeList(area);

        REQUIRE(shapeList.size() == 3);
        REQUIRE( shapeList[0] == RShape::POOO_0 );
        REQUIRE( shapeList[1] == RShape::PPOO_0 );
        REQUIRE( shapeList[2] == RShape::OOOO );
    }

    SECTION("With disposition constraint") {
        auto p5 = Point(1, 0, 0);
        auto p6 = Point(2, 1, 0);
        auto p7 = Point(1, 1, 0);

        auto kp5 = factory->createKeypoint(p5);
        auto kp6 = factory->createKeypoint(p6);
        auto kp7 = factory->createKeypoint(p7);

        auto l5 = factory->createStraightLine(kp1, kp5);
        auto l6 = factory->createStraightLine(kp5, kp6);
        auto l7 = factory->createStraightLine(kp6, kp7);
        auto l8 = factory->createStraightLine(kp7, kp1);

        std::vector<Line*> lines2 { l5, l6, l7, l8 };
        QuadArea a2(lines2);

        a2.lines[0].qtdElements = 10;
        a2.lines[1].qtdElements = 12;
        a2.lines[2].qtdElements = 10;
        a2.lines[3].qtdElements = 10;

        AreaMesh area(a2, 0.1);
        auto shapeList = MeshShapes::generateShapeList(area);

        REQUIRE( shapeList.size() == 4 );
        REQUIRE( shapeList[0] == RShape::POPO_90 );
        REQUIRE( shapeList[1] == RShape::POPO_90 );
        REQUIRE( shapeList[2] == RShape::PPOO_270 );
        REQUIRE( shapeList[3] == RShape::OOOO );
    }
}