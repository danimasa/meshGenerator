#pragma once

#include <memory>
#include <string>
#include "Line.hpp"
#include "KeyPoint.hpp"

class FileInterpreter {
public:
   virtual std::unique_ptr<KeyPoint> interpreteKeypoint(std::string line) = 0; 
   virtual std::unique_ptr<Line> interpreteLine(std::string line) = 0; 
};