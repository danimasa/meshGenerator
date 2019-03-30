#include <catch.hpp>

#include "ArcLine.hpp"
#include "GeometryFactory.hpp"
#include "mathUtils.hpp"

#include <iostream>

using namespace geomlib;

TEST_CASE("ArcLine") {
    const GeometryFactory* factory = GeometryFactory::getDefaultInstance();

    Point initPoint(0, 0, 0);
    Point finalPoint(0, 2, 0);
    Point midPoint(1, 1, 0);

    auto initKp = factory->createKeypoint(initPoint);
    auto finalKp = factory->createKeypoint(finalPoint);

    Vector init_tangent_vector(1, 0, 0);
    Vector final_tangent_vector(-1, 0, 0);

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

    SECTION("Inverted arc line") {
        auto invertedArco = factory->createArcLine(finalKp, initKp, &midPoint, &final_tangent_vector, &init_tangent_vector);
        Point arcInit = *invertedArco->init_point;
        Point arcFinal = *invertedArco->final_point;
        REQUIRE( arcInit == initPoint );
        REQUIRE( arcFinal == finalPoint );
    }

    SECTION("Point in line") {
        REQUIRE_THROWS( arco->pointAtPosition(-0.9) );
        REQUIRE_THROWS( arco->pointAtPosition(1.1) );

        REQUIRE( arco->pointAtPosition(0) == initPoint );
        REQUIRE( arco->pointAtPosition(1) == finalPoint );
        REQUIRE( arco->pointAtPosition(0.5) == midPoint );
    }

    SECTION("is Point in Line") {
        REQUIRE( double_equals(arco->isPointInLine(midPoint), 0.5) == true );
        Point otherPoint(0.766757673, 0.3580633435, 0);
        REQUIRE( double_equals(arco->isPointInLine(otherPoint), 0.278131204725) == true );
        Point notInLine(0.5, 0.5, 0);
        REQUIRE( arco->isPointInLine(notInLine) == -1 );

        Point inCircleNotInArc(-1, 1, 0);
        arco->isPointInLine(inCircleNotInArc);
        REQUIRE( arco->isPointInLine(inCircleNotInArc) == -1 );

        Point pinit (0, 0, 0);
        Point pfinal (-2, 3, 2.5);
        Point pmid (0.648873897, 1.8891278602, 2.5717897355);
        Point pinline (1, 1.25, 2);

        Vector init_vec (0.8208931472, 0.1109315064, 0.5602041072);
        Vector final_vec (-0.8399099769, -0.0824062619, -0.5364330701);

        auto kp1 = factory->createKeypoint(pinit);
        auto kp2 = factory->createKeypoint(pfinal);

        auto arc3d = factory->createArcLine(kp1, kp2, &pmid, &init_vec, &final_vec);
        double length = arc3d->length();
        REQUIRE( double_equals(length, 6.8017956676) == true );
        REQUIRE( double_equals(arc3d->isPointInLine(pinline), 0.402169033073) == true );
    }
}