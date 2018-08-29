#pragma once

#include "Interpreter.hpp"
#include "GeometryList.hpp"

namespace ansyslib {

class LineInterpreter : public geomlib::Interpreter {
public:
    LineInterpreter(geomlib::GeometryList *geomList) :
        geomList(geomList) {}

    int getLinesPerObject() const { return 4; }
    std::string getBlockCode() const { return "20."; }

    geomlib::Geometry* interpret(std::string &block);

private:
    geomlib::GeometryList* geomList;
};

}