#include "catch.hpp"
#include "fakeit.hpp"

#include <vector>

#include "GeometryBuilder.hpp"

using namespace geomlib;
using namespace fakeit;

TEST_CASE("GeometryBuilder", "[builder]") {
    GeometryList list;
    Mock<Interpreter> interpreter;
    Mock<Geometry> geometry;

    Fake(Method(geometry, getGeometryType));
    When(Method(geometry, getID)).AlwaysReturn(1);

    When(Method(interpreter, getLinesPerObject)).AlwaysReturn(2);
    When(Method(interpreter, interpret)).AlwaysReturn(&geometry.get());
    When(Method(interpreter, getBlockCode)).AlwaysReturn("10.");

    std::vector<Interpreter*> interpreters { &interpreter.get() };

    GeometryBuilder builder(&list, interpreters);

    SECTION("Setting the proper states") {
        REQUIRE( builder.getReading() == READING_TYPES::NOTHING );

        builder.readFileLine("     9999.");
        REQUIRE( builder.getReading() == READING_TYPES::BLOCK_TYPE );

        builder.readFileLine("       10.");
        REQUIRE( builder.getReading() == READING_TYPES::ARRAY_LENGTH );

        builder.readFileLine("       54.");
        REQUIRE( builder.getReading() == READING_TYPES::DATA );

        builder.readFileLine("    -9876.");
        REQUIRE( builder.getReading() == READING_TYPES::NOTHING );
    }

    SECTION("Interpreter not found") {
        builder.readFileLine("     9999.");
        builder.readFileLine("       20.");
        REQUIRE( builder.getReading() == READING_TYPES:: NOTHING );
    }

    builder.readFileLine("     9999."); // Init Block
    builder.readFileLine("       10."); // Block Type

    SECTION("Setting proper Array Lenght") {
        builder.readFileLine("       2."); // Array Lenght
        REQUIRE( list.capacity() == 2 );
    }

    builder.readFileLine("       2."); // Array Lenght

    SECTION("Interpreting Data") {
        std::string content = "content";
        std::string mixedContent = "content\ncontent";

        // Read content
        builder.readFileLine(content);
        Verify(Method(interpreter, getLinesPerObject));

        builder.readFileLine(content);
        Verify(Method(interpreter, getLinesPerObject)).Exactly(1);
        Verify(Method(interpreter, interpret)).Exactly(1);

        REQUIRE( list.size() == 1 );
    }
}