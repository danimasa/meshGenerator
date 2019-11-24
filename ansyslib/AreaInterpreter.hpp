#pragma once

#include "Interpreter.hpp"
#include "GeometryList.hpp"

namespace ansyslib {

class AreaInterpreter : public geomlib::Interpreter {
public:
  AreaInterpreter(geomlib::GeometryList *geomList) :
    geomList(geomList) {}

  std::string getBlockCode() const { return "30."; }
  bool belongToCurrentGeometry(const std::string &line);

  geomlib::Geometry* interpret();

private:
  geomlib::GeometryList* geomList;
};

}