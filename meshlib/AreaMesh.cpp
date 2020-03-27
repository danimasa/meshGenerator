#include "AreaMesh.hpp"
#include "meshlib.hpp"
#include "Polyline.hpp"
#include "MeshShapesGenerator.hpp"
#include "MeshShapes.hpp"

#include <math.h>
#include <vector>
#include <functional>

namespace meshlib {

void AreaMesh::determineLinesSubdivision() {
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

Mesh AreaMesh::generateMesh() {
    auto shapeList = MeshShapes::generateShapeList(*this);
    MeshShapesGenerator gen;
    auto regMesh = gen.genMesh(shapeList, *this);
    // return regMesh;
    return transfiniteMapping(regMesh, *this);
}

} // namespace meshlib