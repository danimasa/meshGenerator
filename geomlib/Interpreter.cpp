#include "Interpreter.hpp"

namespace geomlib {

void Interpreter::setAccumulatedLines(const std::string &block) {
    accumulatedLines = block;
    accumulatedLinesCount = std::count(
        accumulatedLines.begin(),
        accumulatedLines.end(),
        '\n');
}

bool Interpreter::accumulateLine(const std::string &line) {
    if(belongToCurrentGeometry(line)) {
        accumulatedLines += '\n' + line;
        accumulatedLinesCount += 1;
        return false;
    }

    return true;
}

void Interpreter::resetAccumulatedLines() {
    accumulatedLinesCount = 0;
    accumulatedLines = "";
}

}