#pragma once

#include "FileStructure.hpp"

class AnsysFileStructure : public FileStructure {
public:
    AnsysFileStructure(FileInterpreter *interpreter, MeshBuilder *builder) :
        FileStructure(interpreter, builder) {}

    void readFileLine(std::string line);
    std::shared_ptr<Mesh> getMesh();
};
