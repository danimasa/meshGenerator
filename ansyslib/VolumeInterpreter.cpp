#include "VolumeInterpreter.hpp"
#include "LineCounter.hpp"

using namespace geomlib;
using namespace std;

namespace ansyslib
{

int VolumeInterpreter::getLinesPerObject(const string &firstLine) const {
    return simplePlusConcatenatedLinesCounter(firstLine);
}

Geometry* VolumeInterpreter::interpret(string &block) {
    return nullptr;
}

}