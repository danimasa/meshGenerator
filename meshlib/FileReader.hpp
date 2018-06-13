#pragma once

#include <string>
#include <Geometry.hpp>

using namespace std;

class FileReader {
public:
    FileReader(const string filePath) : filePath(filePath) {}

    string filePath;

    virtual Geometry* read() = 0;
};