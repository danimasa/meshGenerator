#include <catch.hpp>

#include "DefaultMeshBuilder.hpp"

TEST_CASE("Should create a Mesh") {
    DefaultMeshBuilder builder;

    REQUIRE( builder.getMesh()->Lines.capacity() == 0 );
    REQUIRE( builder.getMesh()->Points.capacity() == 0 );

    SECTION("Should realloc array size for a type") {
        builder.setArraySize(READING_TYPES::KEYPOINTS, 10);
        REQUIRE( builder.getMesh()->Points.capacity() == 10 );

        builder.setArraySize(READING_TYPES::LINES, 10);
        REQUIRE( builder.getMesh()->Lines.capacity() == 10 );
    }

    SECTION("Should add a Keypoint") {
        shared_ptr<KeyPoint> kp (new KeyPoint { 0, 0, 0 });
        builder.AddKeypoint(kp);
        
        shared_ptr<Mesh> mesh = builder.getMesh();

        REQUIRE( mesh.get() != nullptr );
        REQUIRE( mesh.get()->Points.size() == 1 );
    }

    SECTION("Should add a Line") {
        shared_ptr<KeyPoint> init (new KeyPoint( 0, 0, 0));
        shared_ptr<KeyPoint> finish (new KeyPoint( 10, 10, 10));
        shared_ptr<Line> line (new Line { init, finish });
        builder.AddLine(line);

        shared_ptr<Mesh> mesh = builder.getMesh();

        REQUIRE( mesh.get() != nullptr );
        REQUIRE( mesh.get()->Lines.size() == 1 );
    }
}