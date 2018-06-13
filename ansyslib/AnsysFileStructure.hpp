#pragma once

#include "FileStructure.hpp"

class AnsysFileStructure : public FileStructure {
public:
    AnsysFileStructure(FileInterpreter *interpreter, GeometryBuilder *builder) :
        FileStructure(interpreter, builder) {}

    void readFileLine(std::string line);
    std::shared_ptr<Geometry> getGeometry();
};
