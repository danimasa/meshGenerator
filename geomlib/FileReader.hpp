#pragma once

#include <string>
#include "GeometryList.hpp"
#include "FileStructure.hpp"

using namespace std;

namespace geomlib {

    class FileReader {
    public:
        FileReader(FileStructure* structure) : structure(structure) {}

        shared_ptr<GeometryList> read(const string filePath);

    protected:
        FileStructure* structure;
    };
}
