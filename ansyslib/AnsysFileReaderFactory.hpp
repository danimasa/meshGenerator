#pragma once

#include "FileReaderFactory.hpp"

class AnsysFileReaderFactory : public FileReaderFactory {
public:
    FileReader *createReader(const string filePath);
};