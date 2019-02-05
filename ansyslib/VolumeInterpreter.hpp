#pragma once

#include "Interpreter.hpp"
#include "GeometryList.hpp"

namespace ansyslib {

class VolumeInterpreter : public geomlib::Interpreter {
public:
    VolumeInterpreter(geomlib::GeometryList *geomList) :
        geomList(geomList) {}

    int getLinesPerObject(const std::string &first_line = "") const;
    std::string getBlockCode() const { return "40."; }

    geomlib::Geometry* interpret(std::string &block);

private:
    geomlib::GeometryList* geomList;
};

}