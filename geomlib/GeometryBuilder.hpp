#pragma once

#include <list>
#include "GeometryList.hpp"
#include "Interpreter.hpp"

namespace geomlib {

    class GeometryBuilder {
    private:
        Interpreter* interpreter;
        GeometryList* geometryList;
        bool readedArrayLenght = false;
        int accumulated_lines_count = 0;
        std::string accumulated_lines;

    public:
        GeometryBuilder(GeometryList* geomList, Interpreter* interpreter)
            : geometryList(geomList),
              interpreter(interpreter) {}

        void readFileLine(const std::string &line);
    };

}
