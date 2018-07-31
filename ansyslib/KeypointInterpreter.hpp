#pragma once

#include "Interpreter.hpp"

namespace ansyslib {

class KeypointInterpreter : public geomlib::Interpreter {
public:
    int getLinesPerObject() const { return 1; }
    std::string getBlockCode() const { return "10."; }

    geomlib::Geometry* interpret(std::string &block);
};

}