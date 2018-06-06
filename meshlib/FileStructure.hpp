#pragma once

#include "Mesh.hpp"
#include "FileInterpreter.hpp"
#include "MeshBuilder.hpp"

class FileStructure {
protected:
    READING_TYPES reading = READING_TYPES::NOTHING;
    bool readingArraySize = false;
    FileInterpreter *interpreter;
    MeshBuilder *builder;

    FileStructure (FileInterpreter *interpreter, MeshBuilder *builder) :
        interpreter(interpreter), builder(builder) {}

public:
    virtual void readFileLine(std::string line) = 0;
    virtual std::shared_ptr<Mesh> getMesh() = 0;

    READING_TYPES getReading() { return reading; }
    bool isReadingArraySize() { return readingArraySize; }
};