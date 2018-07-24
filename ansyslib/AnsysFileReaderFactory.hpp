#pragma once

#include "FileReaderFactory.hpp"

using namespace geomlib;

namespace ansyslib {

    class AnsysFileReaderFactory : public FileReaderFactory {
    public:
        FileReader *createReader();
    };

}