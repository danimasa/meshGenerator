#include <assert.h>
#include <stdexcept>

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
        l.qtdElements = 0;
        lastLine = l;

        this->lines.push_back(l);
    }
}

bool QuadArea::someQtdElementsZero() const {
    bool someZeroElements = false;

    for(int i = 0; i < 4; i++) {
        if (lines[i].qtdElements == 0)
            someZeroElements = true;
    }

    return someZeroElements;
}

int QuadArea::sumQtdElements() const {
    int sum = 0;
    for(int i = 0; i < 4; i++)
        sum += lines[i].qtdElements;
    return sum;
}

bool QuadArea::isEvenElements() const {
    return sumQtdElements() % 2 == 0;
}

vector<Line*> QuadArea::getLines() const {
    std::vector<Line*> result;
    result.resize(lines.size());
    std::transform(lines.begin(), lines.end(),
        result.begin(),
        [](const QuadArea::TopologicalLine& l) { return l.line; });

    return result;
}

vector<Point*> QuadArea::getVertex() const {
    std::vector<Point*> result;

    Point* lastPoint = nullptr;
    for(auto l : lines) {
        if (lastPoint == nullptr) {
            lastPoint = l.line->init_point;
            result.push_back(l.line->init_point);
            continue;
        }

        if (lastPoint == l.line->init_point) {
            result.push_back(l.line->final_point);
            lastPoint = l.line->final_point;
        } else {
            result.push_back(l.line->init_point);
            lastPoint = l.line->init_point;
        }
    }

    return result;
}

} // namespace geomlib
