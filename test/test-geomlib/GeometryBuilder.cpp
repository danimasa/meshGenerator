#include "catch.hpp"
#include "fakeit.hpp"

#include "GeometryBuilder.hpp"

using namespace geomlib;
using namespace fakeit;

TEST_CASE("GeometryBuilder", "[builder]") {
    GeometryList list;
    Mock<Interpreter> interpreter;
    Mock<Geometry> geometry;

    Fake(Method(geometry, getGeometryType));

    When(Method(interpreter, getLinesPerObject)).AlwaysReturn(2);
    When(Method(interpreter, interpret)).AlwaysReturn(&geometry.get());
    When(Method(interpreter, getBlockCode)).AlwaysReturn("10.");

    GeometryBuilder builder(&list, &interpreter.get());

    std::string content = "content";
    std::string mixedContent = "content\ncontent";

    // Read array lenght
    builder.readFileLine("       2.");
    REQUIRE( list.capacity() == 2 );

    // Read content
    builder.readFileLine(content);
    Verify(Method(interpreter, getLinesPerObject));

    builder.readFileLine(content);
    Verify(Method(interpreter, getLinesPerObject)).Exactly(2);
    Verify(Method(interpreter, interpret)).Exactly(1);

    REQUIRE( list.size() == 1 );
}