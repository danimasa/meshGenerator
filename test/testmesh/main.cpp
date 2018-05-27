#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "AnsysFileReaderFactory.hpp"
#include "AnsysFileReader.hpp"

TEST_CASE("Generate ansys file reader from factory", "[factory]") {
    auto factory = AnsysFileReaderFactory();
    string filePath = "c:/teste";
    auto reader = factory.createReader(filePath);

    auto isFileReader = dynamic_cast<FileReader*>(reader);
    REQUIRE( isFileReader != NULL );

    auto isAnsysFileReader = dynamic_cast<AnsysFileReader*>(isFileReader);
    REQUIRE( isAnsysFileReader != NULL );
    
    REQUIRE( isAnsysFileReader->filePath == filePath );
}