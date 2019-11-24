#pragma once

#include "Interpreter.hpp"
#include "GeometryList.hpp"

namespace ansyslib {

class LineInterpreter : public geomlib::Interpreter {
public:
    LineInterpreter(geomlib::GeometryList *geomList) :
        geomList(geomList) {}

    std::string getBlockCode() const { return "20."; }
    bool belongToCurrentGeometry(const std::string &line);

    geomlib::Geometry* interpret();

private:
    geomlib::GeometryList* geomList;
};

}