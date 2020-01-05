#include <assert.h>

#include "QuadArea.hpp"

namespace geomlib
{

QuadArea::QuadArea(vector<Line*> lines) {

if (lines.size() != 4)
    throw new invalid_argument("A QuadArea must have 4 lines");

TopologicalLine lastLine;
for(int i = 0; i < lines.size(); i++) {
    Line* cLine = lines[i];
    TopologicalLine l;

    if(i == 0) {
        l.direction = LineDirection::DIRECT;
    } else if (i == 1) {
        if (lines[0]->final_point == cLine->init_point) {
            l.direction = LineDirection::DIRECT;
        } else if (lines[0]->final_point == cLine->final_point) {
            l.direction = LineDirection::INVERSE;
        } else {
            throw new invalid_argument("Not touching line or not in sequence for QuadArea");
        }
    } else if (i == 2) {
        if (lastLine.direction == LineDirection::DIRECT) {
            if (lines[1]->final_point == cLine->init_point) {
                l.direction = LineDirection::INVERSE;
            } else if (lines[1]->final_point == cLine->final_point) {
                l.direction = LineDirection::DIRECT;
            } else {
                throw new invalid_argument("Not touching line or not in sequence for QuadArea");
            }
        } else {
            if (lines[1]->init_point == cLine->init_point) {
                l.direction = LineDirection::INVERSE;
            } else if (lines[1]->init_point == cLine->final_point) {
                l.direction = LineDirection::DIRECT;
            } else {
                throw new invalid_argument("Not touching line or not in sequence for QuadArea");
            }
        }
    } else if (i == 3) {
        if (lastLine.direction == LineDirection::DIRECT) {
            if (lines[2]->init_point == cLine->init_point) {
                l.direction = LineDirection::INVERSE;
            } else if (lines[2]->init_point == cLine->final_point) {
                l.direction = LineDirection::DIRECT;
            } else {
                throw new invalid_argument("Not touching line or not in sequence for QuadArea");
            }
        } else {
            if (lines[2]->final_point == cLine->init_point) {
                l.direction = LineDirection::INVERSE;
            } else if (lines[2]->final_point == cLine->final_point) {
                l.direction = LineDirection::DIRECT;
            } else {
                throw new invalid_argument("Not touching line or not in sequence for QuadArea");
            }
        }
    }

    l.line = cLine;
    lastLine = l;

    this->lines.push_back(l);
}

}

vector<Line*> QuadArea::getLines() const {
    std::vector<Line*> result;
    result.resize(lines.size());
    std::transform(lines.begin(), lines.end(),
        result.begin(),
        [](const QuadArea::TopologicalLine& l) { return l.line; });

    return result;
}

} // namespace geomlib
