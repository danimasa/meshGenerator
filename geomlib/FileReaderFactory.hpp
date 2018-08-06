#pragma once

#include "FileReader.hpp"

namespace geomlib {

    class FileReaderFactory {
    public:
        virtual FileReader *createReader() = 0;
    };
}