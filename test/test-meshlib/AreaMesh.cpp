#include <catch.hpp>
#include <vector>

#include "AreaMesh.hpp"
#include "GeometryFactory.hpp"
#include "Vertex.hpp"

using namespace meshlib;
using namespace geomlib;
using namespace std;

TEST_CASE("AreaMesh.hpp") {
    auto factory = GeometryFactory::getDefaultInstance();

    auto p1 = Point(0, 0, 0);
    auto p2 = Point(2, 0, 0);
    auto p3 = Point(2, 2, 0);
    auto p4 = Point(0, 2, 0);

    auto kp1 = factory->createKeypoint(p1);
    auto kp2 = factory->createKeypoint(p2);
    auto kp3 = factory->createKeypoint(p3);
    auto kp4 = factory->createKeypoint(p4);

    auto l1 = factory->createStraightLine(kp1, kp2);
    auto l2 = factory->createStraightLine(kp2, kp3);
    auto l3 = factory->createStraightLine(kp3, kp4);
    auto l4 = factory->createStraightLine(kp4, kp1);

    SECTION("simple mesh") {
        vector<Line*> lines {l1, l2, l3, l4};
        QuadArea a1(lines);

        AreaMesh meshGenerator(0.25);
        Mesh mesh = meshGenerator.generateMesh(&a1);

        REQUIRE( mesh.vertices.size() == 81 );
        REQUIRE( mesh.elements.size() == 64 );
    }

    SECTION("Polyline mesh") {
        auto p5 = Point(1, 2.5, 0);
        auto kp5 = factory->createKeypoint(p5);

        auto l5 = factory->createStraightLine(kp3, kp5);
        auto l6 = factory->createStraightLine(kp5, kp4);
        std::vector<Line*> polylineList { l5, l6 };

        auto l7 = factory->createPolyline(kp3, kp4, polylineList);

        vector<Line*> quadLines { l1, l2, l7, l4 };
        QuadArea a2(quadLines);

        AreaMesh meshGenerator(0.25);
        Mesh mesh = meshGenerator.generateMesh(&a2);

        auto vertices = mesh.vertices;
        Vertex v(p5);
        auto findVertex = std::find_if(vertices.begin(), vertices.end(),
            [&v](const Vertex* vertex){
                return v == *vertex;
            });
        REQUIRE( findVertex != vertices.end() );
    }
}