#pragma once

#include "GeometryList.hpp"
#include "GeometryBuilder.hpp"

using namespace std;

namespace geomlib {

    class FileReader {
    public:
        FileReader(GeometryBuilder* builder) : builder(builder) {}

        GeometryList* read(const string filePath);

    protected:
        GeometryBuilder* builder;
    };
}
