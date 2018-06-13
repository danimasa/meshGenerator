#pragma once

#include "FileReader.hpp"

class AnsysFileReader : public FileReader {
public:
    AnsysFileReader(string filePath) : FileReader(filePath) {}

    Geometry* read();
};