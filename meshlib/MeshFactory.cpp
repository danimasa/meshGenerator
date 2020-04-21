#include "MeshFactory.hpp"
#include "meshlib.hpp"
#include "MeshShapes.hpp"
#include "MeshShapesGenerator.hpp"
#include "Polyline.hpp"

namespace meshlib {

MeshFactory* MeshFactory::getDefaultInstance()
{
    static MeshFactory instance;
    return &instance;
}

vector<Vertex*> MeshFactory::generateLineVertices(
    Line* line,
    int qtdElements,
    double lastXPos,
    double pPosition,
    bool direct
) const {
    vector<Vertex*> vList;

    // if(line->getLineType() == LineType::Polyline) {
    //     auto pLine = dynamic_cast<Polyline*>(line);
    //     auto pLineList = pLine->getLinesWithDirection();
    //     auto iPoints = pLine->intermidiaryPoints();
    //     int lIndex = 0;
    //     int nGenElements = 0;
    //     double lastXPos = 0.0;

    //     for(auto p : iPoints) {
    //         double pPosition = pLine->isPointInLine(*p);
    //         int nElements = ceil(qtdElements * (pPosition - lastXPos));
    //         auto subLine = pLineList[lIndex];
    //         bool direct = subLine.direction == LineDirection::DIRECT;
    //         auto pList = generateLineVertices(subLine.line, nElements, lastXPos, pPosition, direct);
    //         for(auto v : pList)
    //             vList.push_back(v);

    //         auto v = genVertex(p);
    //         vList.push_back(v);
    //         lIndex++;
    //         nGenElements += nElements;
    //         lastXPos = pPosition;
    //     }

    //     int nElements = qtdElements - nGenElements;
    //     auto subLine = pLineList[lIndex];
    //     bool direct = subLine.direction == LineDirection::DIRECT;
    //     auto pList = generateLineVertices(subLine.line, nElements, lastXPos, direct);
    //     for(auto v : pList)
    //         vList.push_back(v);

    // } else {
    double step = 1.0 / qtdElements;
    for (int i = 1; i < qtdElements; i++) {
        double pos = lastXPos + (i * step) * (pPosition - lastXPos);
        if (!direct) pos = 1.0 - pos;
        auto p = line->pointAtPosition(pos);
        auto v = genVertex(&p);
        vList.push_back(v);
    }
    // }

    return vList;
}

void MeshFactory::setElementSize(double elementSize) {
    this->elementSize = elementSize;
}

Vertex* MeshFactory::genVertex(Point *point) const {
    auto vertex = new Vertex(point);
    return vertex;
}

Vertex* MeshFactory::genVertex(double x, double y, double z) const {
    auto vertex = new Vertex(x, y, z);
    return vertex;
}

vector<Vertex*> MeshFactory::subdivideLine(Line *line, int qtdElements) const {
    return generateLineVertices(line, qtdElements);
}

vector<Vertex*> MeshFactory::subdivideLine(Line *line) const {
    if (elementSize <= 0.0)
        throw "MeshFactory subdivideLine, necessary set elementSize first";

    int qtdElements = ceil(line->length() / elementSize);
    return subdivideLine(line, qtdElements);
}

Mesh* MeshFactory::genSurfaceMesh(QuadArea *area) const {
    auto shapeList = MeshShapes::generateShapeList(*area);
    MeshShapesGenerator gen;
    auto regMesh = gen.genMesh(shapeList, *area);
    auto mesh = transfiniteMapping(regMesh, *area);
    return mesh;
}

QuadArea* MeshFactory::subdivideArea(Area *area) const {
    auto qArea = new QuadArea(area->loops[0]->lines);
    return qArea;
}

}