#include <catch.hpp>

#include "ArcLine.hpp"
#include "GeometryFactory.hpp"

#include <iostream>

using namespace geomlib;

TEST_CASE("ArcLine") {
    const GeometryFactory* factory = GeometryFactory::getDefaultInstance();

    Point initPoint(0, 0, 0);
    Point finalPoint(0, 2, 0);
    Point midPoint(1, 1, 0);

    auto initKp = factory->createKeypoint(initPoint);
    auto finalKp = factory->createKeypoint(finalPoint);

    Vector init_tangent_vector(-1, 0, 0);
    Vector final_tangent_vector(init_tangent_vector);

    ArcLine* arco = factory->createArcLine(initKp, finalKp, &midPoint, &init_tangent_vector, &final_tangent_vector);

    // Validando factory
    double epsilon = 0.00000000000000000001;

    REQUIRE( (arco->get_radius() - 1.0) < epsilon );
    REQUIRE( arco->get_center()->x == 0. );
    REQUIRE( (arco->get_center()->y - 1.0) < epsilon );
    REQUIRE( arco->get_center()->z == 0. );

    // Calculo do comprimento
    double length = arco->length();
    REQUIRE( (length - M_PI) < epsilon );

    SECTION("Point in line") {
        REQUIRE_THROWS( arco->pointInLine(-0.9) );
        REQUIRE_THROWS( arco->pointInLine(1.1) );

        REQUIRE( arco->pointInLine(0) == initPoint );
        REQUIRE( arco->pointInLine(1) == finalPoint );
        REQUIRE( arco->pointInLine(0.5) == midPoint );
    }
}