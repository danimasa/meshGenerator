#pragma once

#include "Geometry.hpp"
#include "FileInterpreter.hpp"
#include "GeometryBuilder.hpp"

class FileStructure {
protected:
    READING_TYPES reading = READING_TYPES::NOTHING;
    bool readingArraySize = false;
    FileInterpreter *interpreter;
    GeometryBuilder *builder;

    FileStructure (FileInterpreter *interpreter, GeometryBuilder *builder) :
        interpreter(interpreter), builder(builder) {}

public:
    virtual void readFileLine(std::string line) = 0;
    virtual std::shared_ptr<Geometry> getGeometry() = 0;

    READING_TYPES getReading() { return reading; }
    bool isReadingArraySize() { return readingArraySize; }
};