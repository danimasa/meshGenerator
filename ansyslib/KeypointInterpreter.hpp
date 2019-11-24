#pragma once

#include "Interpreter.hpp"

namespace ansyslib {

class KeypointInterpreter : public geomlib::Interpreter {
public:
    KeypointInterpreter() {}

    std::string getBlockCode() const { return "10."; }
    bool belongToCurrentGeometry(const std::string &line);

    geomlib::Geometry* interpret();
};

}