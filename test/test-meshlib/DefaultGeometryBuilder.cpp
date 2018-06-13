#include <catch.hpp>

#include "DefaultGeometryBuilder.hpp"

TEST_CASE("Should create a Geometry") {
    DefaultGeometryBuilder builder;

    REQUIRE( builder.getGeometry()->Lines.capacity() == 0 );
    REQUIRE( builder.getGeometry()->Points.capacity() == 0 );

    SECTION("Should realloc array size for a type") {
        builder.setArraySize(READING_TYPES::KEYPOINTS, 10);
        REQUIRE( builder.getGeometry()->Points.capacity() == 10 );

        builder.setArraySize(READING_TYPES::LINES, 10);
        REQUIRE( builder.getGeometry()->Lines.capacity() == 10 );
    }

    SECTION("Should add a Keypoint") {
        shared_ptr<KeyPoint> kp (new KeyPoint { 0, 0, 0 });
        builder.AddKeypoint(kp);
        
        shared_ptr<Geometry> geometry = builder.getGeometry();

        REQUIRE( geometry.get() != nullptr );
        REQUIRE( geometry.get()->Points.size() == 1 );
    }

    SECTION("Should add a Line") {
        shared_ptr<KeyPoint> init (new KeyPoint( 0, 0, 0));
        shared_ptr<KeyPoint> finish (new KeyPoint( 10, 10, 10));
        shared_ptr<Line> line (new Line { init, finish });
        builder.AddLine(line);

        shared_ptr<Geometry> geometry = builder.getGeometry();

        REQUIRE( geometry.get() != nullptr );
        REQUIRE( geometry.get()->Lines.size() == 1 );
    }
}