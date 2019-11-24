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
        bool result = currentInterpreter->accumulateLine(l_line);

        if (result) {
            auto geometry = currentInterpreter->interpret();
            geometryList->add(geometry);
            currentInterpreter->resetAccumulatedLines();
            currentInterpreter->accumulateLine(l_line);
        }
    }
} 

}