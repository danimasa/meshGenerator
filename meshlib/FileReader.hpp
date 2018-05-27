#pragma once

#include <string>
#include <Mesh.hpp>

using namespace std;

class FileReader {
public:
    FileReader(const string filePath) : filePath(filePath) {}

    string filePath;

    virtual Mesh* read() = 0;
};