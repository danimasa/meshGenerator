#define _USE_MATH_DEFINES
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
    REQUIRE( arco->get_init_vector() == init_tangent_vector );
    REQUIRE( arco->get_final_vector() == final_tangent_vector );

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
        auto mypoint = arco->pointAtPosition(0.5);
        REQUIRE( double_equals(mypoint.x, midPoint.x) );
        REQUIRE( double_equals(mypoint.y, midPoint.y) );
        REQUIRE( double_equals(mypoint.z, midPoint.z) );
    }
    
    SECTION("Point in line multiple angles") {
        Point pinit (2, 0, 0);
        Point pfinal (1.9590536134908, -0.4026275443494, 0);
        Point midpoint (-1.9897370714471, 0.2023521349263, 0);

        Vector initvec (0, 1, 0);
        Vector finalvec (0.2013137721747, 0.9795268067454, 0);

        auto kp1 = factory->createKeypoint(pinit);
        auto kp2 = factory->createKeypoint(pfinal);

        auto arc = factory->createArcLine(kp1, kp2, &midpoint, &initvec, &finalvec);

        // between 0 and 90 degrees
        auto ipoint = arc->pointAtPosition(0.15);
        REQUIRE( double_equals(ipoint.x, 1.2242156590063) );
        REQUIRE( double_equals(ipoint.y, 1.5815486145686) );

        // between 90 and 180 degrees
        ipoint = arc->pointAtPosition(0.4);
        REQUIRE( double_equals(ipoint.x, -1.5175081314194) );
        REQUIRE( double_equals(ipoint.y, 1.3027544170242) );

        // between 180 and 270 degrees
        ipoint = arc->pointAtPosition(0.65);
        REQUIRE( double_equals(ipoint.x, -1.3779485094093) );
        REQUIRE( double_equals(ipoint.y, -1.4495716282464) );

        // between 270 and 360 degrees
        ipoint = arc->pointAtPosition(0.9);
        REQUIRE( double_equals(ipoint.x, 1.377913523869) );
        REQUIRE( double_equals(ipoint.y, -1.4496048843526) );

        // 90 degrees
        ipoint = arc->pointAtPosition(0.2583339948706425);
        REQUIRE( double_equals(ipoint.x, 0) );
        REQUIRE( double_equals(ipoint.y, 2) );

        // 180 degress
        ipoint = arc->pointAtPosition(0.51666798974128925);
        REQUIRE( double_equals(ipoint.x, -2) );
        REQUIRE( double_equals(ipoint.y, 0) );

        // 270 degress
        ipoint = arc->pointAtPosition(0.77500198461197434);
        REQUIRE( double_equals(ipoint.x, 0) );
        REQUIRE( double_equals(ipoint.y, -2) );
    }

    SECTION("is Point in Line") {
        REQUIRE( double_equals(arco->isPointInLine(midPoint), 0.5) == true );
        Point otherPoint(0.766757673, 0.3580633435, 0);
        REQUIRE( double_equals(arco->isPointInLine(otherPoint), 0.278131204725) == true );
        Point notInLine(0.5, 0.5, 0);
        REQUIRE( arco->isPointInLine(notInLine) == -1 );

        Point inCircleNotInArc(-1, 1, 0);
        Point p90(1, 1, 0);
        REQUIRE( arco->isPointInLine(inCircleNotInArc) == -1 );

        Point pinit (0, 0, 0);
        Point pfinal (-2, 3, 2.5);
        Point pmid (0.7844639004, 1.707733393, 2.4282910487);
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

    SECTION("OutBox") {
        // Simple arc
        Box box = arco->outBox();
        Point center = box.get_center();

        REQUIRE( double_equals(center.x, 0.5) == true );
        REQUIRE( double_equals(center.y, 1) == true );
        REQUIRE( center.z == 0 );

        REQUIRE( double_equals(box.get_height(), 2) == true );
        REQUIRE( box.get_depth() == 0 );
        REQUIRE( double_equals(box.get_width(), 1) == true );

        // Complex arc
        Point c_ip(1, 2, 3);
        Point c_fp(6, 5, 4);
        Point c_mp(5, 2, 3);

        auto c_ikp = factory->createKeypoint(c_ip);
        auto c_fkp = factory->createKeypoint(c_fp);

        Vector c_itv(-0.7644707872, 0.6115766297, 0.2038588766);
        Vector c_ftv(-0.2548235957, 0.9173649446, 0.3057883149);

        ArcLine* c_arc = factory->createArcLine(c_ikp, c_fkp, &c_mp, &c_itv, &c_ftv);

        Point verify_point(-0.0907590629, 3.9950733958, 3.6650244653);
        double inCircle = c_arc->isPointInLine(verify_point);
        REQUIRE( inCircle == -1 );

        Box c_box = c_arc->outBox();
        Point c_box_center = c_box.get_center();

        REQUIRE( double_equals(c_box_center.x, 3.5512092056620714) );
        REQUIRE( double_equals(c_box_center.y, 3.1533937347673793) );
        REQUIRE( double_equals(c_box_center.z, 3.3844645782557929) );

        REQUIRE( double_equals(c_box.get_height(), 3.6932125305) == true );
        REQUIRE( double_equals(c_box.get_depth(), 1.2310708435) == true );
        REQUIRE( double_equals(c_box.get_width(), 5.1024184113) == true );
    }
}