#include <assert.h>
#include <stdexcept>

#include "QuadArea.hpp"

namespace meshlib
{

QuadArea::QuadArea(vector<LineMesh*> lines) {
    if (lines.size() != 4)
        throw invalid_argument("A QuadArea must have 4 lines");

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
                throw invalid_argument("Not touching line or not in sequence for QuadArea");
            }
        } else if (i == 2) {
            if (lastLine.direction == LineDirection::DIRECT) {
                if (lines[1]->final_point == cLine->init_point) {
                    l.direction = LineDirection::INVERSE;
                } else if (lines[1]->final_point == cLine->final_point) {
                    l.direction = LineDirection::DIRECT;
                } else {
                    throw invalid_argument("Not touching line or not in sequence for QuadArea");
                }
            } else {
                if (lines[1]->init_point == cLine->init_point) {
                    l.direction = LineDirection::INVERSE;
                } else if (lines[1]->init_point == cLine->final_point) {
                    l.direction = LineDirection::DIRECT;
                } else {
                    throw invalid_argument("Not touching line or not in sequence for QuadArea");
                }
            }
        } else if (i == 3) {
            if (lastLine.direction == LineDirection::DIRECT) {
                if (lines[2]->init_point == cLine->init_point) {
                    l.direction = LineDirection::INVERSE;
                } else if (lines[2]->init_point == cLine->final_point) {
                    l.direction = LineDirection::DIRECT;
                } else {
                    throw invalid_argument("Not touching line or not in sequence for QuadArea");
                }
            } else {
                if (lines[2]->final_point == cLine->init_point) {
                    l.direction = LineDirection::INVERSE;
                } else if (lines[2]->final_point == cLine->final_point) {
                    l.direction = LineDirection::DIRECT;
                } else {
                    throw invalid_argument("Not touching line or not in sequence for QuadArea");
                }
            }
        }

        l.line = cLine;
        l.qtdElements = 0;
        lastLine = l;

        this->lines.push_back(l);
    }
}

QuadArea::QuadArea(vector<LineMesh*> lines, double attenuationAngleRatio) : QuadArea(lines) {
    this->attenuationAngleRatio = attenuationAngleRatio;
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

vector<LineMesh*> QuadArea::getLines() const {
    std::vector<LineMesh*> result;
    result.resize(lines.size());
    std::transform(lines.begin(), lines.end(),
        result.begin(),
        [](const QuadArea::TopologicalLine& l) { return l.line; });

    return result;
}

vector<Vertex*> QuadArea::getVertex() const {
    std::vector<Vertex*> result;

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

void QuadArea::determineLinesSubdivision(double elementSize) {
    int sum = sumQtdElements();
    bool someNonZero = someQtdElementsZero();
    if(sum % 2 == 0 && !someNonZero) return;

    int leIndex = 0;
    int newQtdElements = 0;
    double leastDiff = 0.0;
    for(int i = 0; i < 4; i++) {
        QuadArea::TopologicalLine& tLine = lines[i];
        if (tLine.qtdElements == 0)
            tLine.qtdElements = std::ceil(tLine.line->length() / elementSize);
        else if(!someNonZero) {
            double nf = tLine.line->length() / elementSize;
            int upElements = tLine.qtdElements + 1;
            int downElements = tLine.qtdElements - 1;
            double upDiff = std::abs((nf - upElements) / upElements);
            double downDiff = std::abs((nf - downElements) / downElements);
            double lDiff = upDiff <= downDiff ? upDiff : downDiff;
            double dElements = upDiff <= downDiff ? upElements : downElements;
            if (lDiff <= leastDiff || leastDiff == 0.0) {
                leIndex = i;
                leastDiff = lDiff;
                newQtdElements = dElements;
            }
        }
    }

    if(someNonZero) determineLinesSubdivision();
    else lines[leIndex].qtdElements = newQtdElements;
}

} // namespace geomlib
