#pragma once

#include <string>
#include "GeometryList.hpp"

using namespace std;

namespace geomlib {

    class FileReader {
    public:
        FileReader(const string filePath) : filePath(filePath) {}

        virtual shared_ptr<GeometryList> read() = 0;

    private:
        string filePath;
    };
}
