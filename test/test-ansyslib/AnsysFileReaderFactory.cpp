#include "catch.hpp"

#include "AnsysFileReaderFactory.hpp"
#include "AnsysFileStructure.hpp"

using namespace ansyslib;

TEST_CASE("Generate ansys file reader from factory", "[factory]") {
    auto factory = AnsysFileReaderFactory();

    SECTION("Should create FileStructure") {
        auto structure = factory.createFileStructure();

        auto isFileStructure = dynamic_cast<AnsysFileStructure*>(structure);
        REQUIRE( isFileStructure != NULL );
    }
    
    SECTION("Should create FileReader") {
        auto reader = factory.createReader();

        auto isFileReader = dynamic_cast<FileReader*>(reader);
        REQUIRE( isFileReader != NULL );
    }
}