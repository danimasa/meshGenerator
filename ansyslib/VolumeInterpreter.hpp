#pragma once

#include "Interpreter.hpp"
#include "GeometryList.hpp"

namespace ansyslib {

class VolumeInterpreter : public geomlib::Interpreter {
public:
    VolumeInterpreter(geomlib::GeometryList *geomList) :
        geomList(geomList) {}

    std::string getBlockCode() const { return "40."; }
    bool belongToCurrentGeometry(const std::string &line);

    geomlib::Geometry* interpret();

private:
    geomlib::GeometryList* geomList;
    int totalLines;
};

}