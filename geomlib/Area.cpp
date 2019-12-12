#include "Area.hpp"

namespace geomlib {

Area::Loop::Loop(vector<Line*> lines) {

Line* lastLine = nullptr;
LineDirection lastLineDirection;
for(auto currLine : lines) {
    Area::LoopLine l;

    if(lastLine == nullptr) {
        l.direction = LineDirection::DIRECT;
    } else if (lastLineDirection == LineDirection::DIRECT) {
        if (lastLine->final_point == currLine->init_point) {
            l.direction = LineDirection::DIRECT;
        } else {
            l.direction = LineDirection::INVERSE;
        }
    } else {
        if (lastLine->final_point == currLine->init_point) {
            l.direction = LineDirection::INVERSE;
        } else {
            l.direction = LineDirection::DIRECT;
        }
    }
    l.line = currLine;
    lastLine = currLine;
    lastLineDirection = l.direction;

    this->lines.push_back(l);
}

}

vector<Line*> Area::Loop::getLines() const {
    std::vector<Line*> result;
    result.resize(lines.size());
    std::transform(lines.begin(), lines.end(),
        result.begin(),
        [](const Area::LoopLine& l) { return l.line; });

    return result;
}

}