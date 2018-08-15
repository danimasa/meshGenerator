#pragma once

#include "Interpreter.hpp"

namespace ansyslib {

class LineInterpreter : public geomlib::Interpreter {
public:
    int getLinesPerObject() const { return 4; }
    std::string getBlockCode() const { return "20."; }

    geomlib::Geometry* interpret(std::string &block);
};

}