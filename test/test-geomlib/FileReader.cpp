// #include "catch.hpp"
// #include "fakeit.hpp"
// #include <cstdio>
// #include <iostream>
// #include <fstream>
// #include <memory>

// #include "FileReader.hpp"

// using namespace geomlib;
// using namespace fakeit;
// using namespace std;

// TEST_CASE("FileReader.cpp") {
//     Mock<FileStructure> structure;

//     Fake(Method(structure, readFileLine));
//     Fake(Method(structure, getBuilders));

//     // Write test file
//     string fileContent { "teste\nde\nleitura" };
//     string filePath { std::tmpnam(nullptr) };
//     ofstream tmpFile;
//     tmpFile.open(filePath);
//     tmpFile << fileContent;
//     tmpFile.close();

//     // Create FileReader object
//     FileReader reader(&structure.get());

//     SECTION("Should read file content") {
//         std::shared_ptr<GeometryList> geom = reader.read(filePath);
//         Verify(Method(structure, readFileLine)).Exactly(3);
//     }

//     // Delete tempFile
//     remove(filePath.c_str());

// }