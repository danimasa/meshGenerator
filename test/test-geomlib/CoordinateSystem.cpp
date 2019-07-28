#include <catch.hpp>

#include "CoordinateSystem.hpp"
#include "GeometryFactory.hpp"
#include "mathUtils.hpp"

using namespace geomlib;

TEST_CASE("CoordinateSystem") {
    const GeometryFactory* factory = GeometryFactory::getDefaultInstance();

    Vector v1(1, 1, 0);
    Vector v2(-1, 1, 0);
    Vector v3 = v1.vectorProduct(v2);

    Point p(0, 1, 0);

    CoordinateSystem* coord = factory->createCoordinateSystem(&v1, &v2, &v3);

    Point p_coord = coord->convert(&p);

    REQUIRE( double_equals(p_coord.x, 0.7071067812) );
    REQUIRE( double_equals(p_coord.y, 0.7071067812) );
    REQUIRE( p_coord.z == 0 );

    Vector vc1(1, 0, 0);
    Vector vc2(0, 1, 0);
    Vector vc3(0, 0, 1);

    CoordinateSystem* canonical_coord = factory->createCoordinateSystem(&vc1, &vc2, &vc3);
    Point p_canonical = canonical_coord->convert(&p_coord, coord);

    REQUIRE( p_canonical.x == 0 );
    REQUIRE( p_canonical.y == 1 );
    REQUIRE( p_canonical.z == 0 );
}