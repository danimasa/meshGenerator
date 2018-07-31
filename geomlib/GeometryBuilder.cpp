#include "GeometryBuilder.hpp"
#include "stringUtils.hpp"

namespace geomlib {

void GeometryBuilder::readFileLine(const std::string &line) {
    std::string l_line = line;
    trim(l_line);

    if (!readedArrayLenght) {
        int size = std::stoi(l_line);
        geometryList->reserveSize(size);
        readedArrayLenght = true;
        return;
    }

    accumulated_lines = accumulated_lines + l_line + "\n";
    accumulated_lines_count++;

    int linesPerObject = interpreter->getLinesPerObject();
    if (accumulated_lines_count == linesPerObject) {
        auto geometry = interpreter->interpret(accumulated_lines);
        geometryList->add(geometry);
    }
} 

}