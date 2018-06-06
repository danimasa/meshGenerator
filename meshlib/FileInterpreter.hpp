#pragma once

#include <string>
#include "Line.hpp"
#include "KeyPoint.hpp"

class FileInterpreter {
public:
   virtual KeyPoint* interpreteKeypoint(std::string line) = 0; 
   virtual Line* interpreteLine(std::string line) = 0; 
};