#pragma once

#include "FileInterpreter.hpp"

class AnsysFileInterpreter : public FileInterpreter {
public:
    std::unique_ptr<KeyPoint> interpreteKeypoint(std::string line);
    std::unique_ptr<Line> interpreteLine(std::string line);
};