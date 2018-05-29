#pragma once

#include <string>
#include <memory>
#include "Mesh.hpp"
#include "FileIntepreter.hpp"

class FileStructure {
private:
    std::string reading;
    FileInterpreter interpreter;
    // MeshBuilder builder;

public:
    // FileStructure (const FileInterpreter &interpreter, const MeshBuilder &builder) :
    //     interpreter(interperter), builder(builder) {}

    virtual void readFileLine(std::string line) = 0;
    virtual std::unique_ptr<Mesh> getMesh() = 0;
};