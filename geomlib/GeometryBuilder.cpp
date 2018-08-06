#include "GeometryBuilder.hpp"
#include "stringUtils.hpp"

namespace geomlib {

void GeometryBuilder::readFileLine(const std::string &line) {
    std::string l_line = line;
    trim(l_line);

    if (reading_state == READING_TYPES::NOTHING) {
        if (l_line == "9999.") {
            reading_state = READING_TYPES::BLOCK_TYPE;
            return;
        }
    }

    if (l_line == "-9876.") {
        reading_state = READING_TYPES::NOTHING;
        return;
    }

    if (reading_state == READING_TYPES::BLOCK_TYPE) {
        for(auto i = 0; i < interpreters.size(); i++) {
            auto interpreter = interpreters.at(i);
            if (interpreter->getBlockCode() == l_line) {
                currentInterpreter = interpreter;
                reading_state = READING_TYPES::ARRAY_LENGTH;
                return;
            }
        }
        reading_state = READING_TYPES::NOTHING;
        return;
    }

    if (reading_state == READING_TYPES::ARRAY_LENGTH) {
        int size = std::stoi(l_line);
        geometryList->reserveSize(size);
        reading_state = READING_TYPES::DATA;
        return;
    }

    if (reading_state == READING_TYPES::DATA) {
        accumulated_lines = accumulated_lines + l_line + "\n";
        accumulated_lines_count++;

        int linesPerObject = currentInterpreter->getLinesPerObject();
        if (accumulated_lines_count == linesPerObject) {
            auto geometry = currentInterpreter->interpret(accumulated_lines);
            geometryList->add(geometry);
            accumulated_lines_count = 0;
            accumulated_lines = "";
        }
    }
} 

}