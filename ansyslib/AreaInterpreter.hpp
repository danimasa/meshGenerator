#pragma once

#include "Interpreter.hpp"
#include "GeometryList.hpp"

namespace ansyslib {

class AreaInterpreter : public geomlib::Interpreter {
public:
  AreaInterpreter(geomlib::GeometryList *geomList) :
    geomList(geomList) {}

  int getLinesPerObject(const std::string &firstLine) const;
  std::string getBlockCode() const { return "30."; }

  geomlib::Geometry* interpret(std::string &block);

private:
  geomlib::GeometryList* geomList;
};

}