#pragma once

#include "FileReader.hpp"

class FileReaderFactory {
public:
    virtual FileReader *createReader(string filePath) = 0;
};