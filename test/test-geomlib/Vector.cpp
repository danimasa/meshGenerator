#define _USE_MATH_DEFINES
#include <catch.hpp>

#include "Vector.hpp"

using namespace geomlib;

TEST_CASE("Vector") {
    // Norma
    Vector v(1, 0, 0);
    REQUIRE( v.norm() == 1.0 );

    // Angle With
    Vector v1(0, 1, 0);
    double angle = v.angleWith(v1);
    REQUIRE( angle == M_PI / 2 );

    // Produto Vetorial
    Vector v3 = v.vectorProduct(v1);
    REQUIRE( v3.angleWith(v) == M_PI / 2 );
    REQUIRE( v3.angleWith(v1) == M_PI / 2 );

    // Normalizacao
    Vector v4(2, 0, 0);
    v4.normalise();
    REQUIRE( v4 == v );

    // Vetor Nulo
    Vector v5(0, 0, 0);
    REQUIRE(v5.isNull() == true);
    REQUIRE(v4.isNull() == false);

    SECTION("Operadores") {
        // Soma
        Vector v1(1, 1, 1);
        Vector v2(2, 2, 2);
        Vector v3 = v1 + v2;
        Vector sum(3, 3, 3);
        REQUIRE( v3 == sum );

        // Subtração
        Vector v4 = v2 - v1;
        Vector sub(1, 1, 1);
        REQUIRE( v4 == sub );

        // Multiplicação
        Vector v5 = v1 * 5;
        Vector mul(5, 5, 5);
        REQUIRE( v5 == mul );
    }

    SECTION("Invert") {
        Vector v1(1, 1, 1);
        Vector v1_inverted(-1, -1, -1);

        REQUIRE( v1.invert() == v1_inverted );
    }
}