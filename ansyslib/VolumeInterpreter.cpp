#include "VolumeInterpreter.hpp"
#include "LineCounter.hpp"

using namespace geomlib;
using namespace std;

namespace ansyslib
{

bool VolumeInterpreter::belongToCurrentGeometry(const string &line) {
    if(accumulatedLinesCount == 0) {
        totalLines = simplePlusConcatenatedLinesCounter(line);
        return true;
    }
    return accumulatedLinesCount < totalLines;
}

Geometry* VolumeInterpreter::interpret() {
    return nullptr;
}

}