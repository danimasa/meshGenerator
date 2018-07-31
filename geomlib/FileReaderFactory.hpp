#pragma once

#include "FileReader.hpp"
#include "FileStructure.hpp"

namespace geomlib {

    class FileReaderFactory {
    public:
        virtual FileReader *createReader() = 0;
        virtual FileStructure *createFileStructure() = 0;
    };
}