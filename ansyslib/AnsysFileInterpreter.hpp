#pragma once

#include "FileInterpreter.hpp"

class AnsysFileInterpreter : public FileInterpreter {
public:
    KeyPoint* interpreteKeypoint(std::string line);
    Line* interpreteLine(std::string line);
};