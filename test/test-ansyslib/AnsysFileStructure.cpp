#include <catch.hpp>
#include <fakeit.hpp>
#include <memory>

#include "AnsysFileStructure.hpp"

using namespace fakeit;

// TEST_CASE("AnsysFileStrucuture") {
//     Mock<FileInterpreter> interpreter;
//     Mock<GeometryBuilder> builder;

//     Fake(Method(interpreter, interpreteKeypoint));
//     Fake(Method(interpreter, interpreteLine));

//     Fake(Method(builder, AddKeypoint));
//     Fake(Method(builder, AddLine));
//     Fake(Method(builder, setArraySize));

//     AnsysFileStructure structure (&interpreter.get(), &builder.get());

//     SECTION("Should begin reading Block") {
//         structure.readFileLine("9999.");

//         REQUIRE( structure.getReading() == READING_TYPES::BLOCK );
//     }

//     SECTION("Should stop reading Block") {
//         structure.readFileLine("9999.");
//         structure.readFileLine("-9876.");

//         REQUIRE( structure.getReading() == READING_TYPES::NOTHING );
//     }

//     SECTION("Outside block continues reading NOTHING") {
//         structure.readFileLine("10.");

//         REQUIRE(structure.getReading() == READING_TYPES::NOTHING );
//     }

//     structure.readFileLine("9999.");
    
//     SECTION("Should read Keypoints") {
//         structure.readFileLine("10.");

//         REQUIRE( structure.getReading() == READING_TYPES::KEYPOINTS );
//         REQUIRE( structure.isReadingArraySize() == true );

//         std::string sizeLine = "25.";
//         structure.readFileLine(sizeLine);
//         Verify(Method(builder, setArraySize).Using(READING_TYPES::KEYPOINTS, 25)).Once();
//         REQUIRE( structure.isReadingArraySize() == false );
        
//         std::string line = "keypoint";
//         structure.readFileLine(line);
//         Verify(Method(interpreter, interpreteKeypoint).Using(line)).Once();
//         Verify(Method(builder, AddKeypoint)).Once();
//     }

//     SECTION("Should read Lines") {
//         structure.readFileLine("20.");

//         REQUIRE( structure.getReading() == READING_TYPES::LINES );
//         REQUIRE( structure.isReadingArraySize() == true );

//         std::string sizeLine = "10.";
//         structure.readFileLine(sizeLine);
//         Verify(Method(builder, setArraySize).Using(READING_TYPES::LINES, 10)).Once();
//         REQUIRE( structure.isReadingArraySize() == false );

//         std::string line = "line";
//         structure.readFileLine(line);
//         Verify(Method(interpreter, interpreteLine).Using(line)).Once();
//         Verify(Method(builder, AddLine)).Once();
//     }

//     SECTION("Should read Areas") {
//         structure.readFileLine("30.");

//         REQUIRE( structure.getReading() == READING_TYPES::AREAS );
//         REQUIRE( structure.isReadingArraySize() == true );

//         std::string sizeLine = "15.";
//         structure.readFileLine(sizeLine);
//         Verify(Method(builder, setArraySize).Using(READING_TYPES::AREAS, 15)).Once();
//         REQUIRE( structure.isReadingArraySize() == false );

//         // std::string line = "area";
//         // structure.readFileLine(line);
//         // Verify(Method(interpreter, interpreteArea).Using(line)).Once();
//         // Verify(Method(builder, AddArea)).Once();
//     }
// }