#include "MeshShapesGenerator.hpp"

#include <stdexcept>

#include "meshlib.hpp"
#include "MeshManager.hpp"

namespace meshlib {

using RShapes = MeshShapes::RelativeShapes;
using VertexLine = vector<Vertex*>;

enum class VertexLineDirection {
    Vertical, Horizontal
};

struct VertexLineParams {
    Vertex* start;
    Vertex* end;
    std::size_t qtdElements;
    double elementsSize;
    VertexLineDirection direction;
};

VertexLine generateVertexLine(VertexLineParams params, Mesh &mesh) {
    VertexLine line;
    line.push_back(params.start);
    int n = params.qtdElements - 1;

    double initPos, finalPos;
    if(params.direction == VertexLineDirection::Horizontal) {
        initPos = params.start->y;
        finalPos = params.end->y;
    } else {
        initPos = params.start->x;
        finalPos = params.end->x;
    }

    double cXPos = params.start->x;
    double cYPos = params.start->y;

    for(int i = 1; i < n; i++) {
        cXPos = params.direction == VertexLineDirection::Horizontal
            ? cXPos + params.elementsSize
            : initPos + i * (finalPos - initPos) / n;
        cYPos = params.direction == VertexLineDirection::Vertical
            ? cYPos + params.elementsSize
            : initPos + i * (finalPos - initPos) / n;
        auto v = new Vertex(cXPos, cYPos, 0);
        mesh.addVertex(v);
        line.push_back(v);
    }
    line.push_back(params.end);

    return line;
}

double getElementSize(const Vertex *v1, const Vertex *v2, VertexLineDirection direction, std::size_t qtdElements) {
    double Vertex::*coord = direction == VertexLineDirection::Horizontal ? &Vertex::x : &Vertex::y;
    double length = v1->*coord - v2->*coord;
    if (length < 0) length = length * -1;
    return length / (qtdElements - 1);
}

Vertex* getRefVertex(const VertexLine &hLine, const VertexLine &vLine) {
    auto hLFirst = *hLine.front();
    auto hLLast = *hLine.back();
    auto vLFirst = *vLine.front();
    auto vLLast = *vLine.back();

    double hElementSize = getElementSize(
        hLine[0],
        hLine[hLine.size() - 1],
        VertexLineDirection::Horizontal,
        hLine.size() + 1
    );

    double vElementSize = getElementSize(
        vLine[0],
        vLine[vLine.size() - 1],
        VertexLineDirection::Vertical,
        vLine.size() + 1
    );

    double xPos = 0.0;
    double yPos = 0.0;

    if (hLFirst.Point::operator==(vLFirst)) {
        xPos = hLFirst.x + hElementSize;
        yPos = hLFirst.y + vElementSize;
    } else if (hLFirst.Point::operator==(vLLast)) {
        xPos = hLFirst.x + hElementSize;
        yPos = hLFirst.y - vElementSize;
    } else if (hLLast.Point::operator==(vLFirst)) {
        xPos = hLLast.x - hElementSize;
        yPos = hLLast.y + vElementSize;
    } else {
        xPos = hLLast.x - hElementSize;
        yPos = hLLast.y - vElementSize;
    }

    auto vertex = new Vertex(xPos, yPos, 0.0);
    return vertex;
}

void MeshShapesGenerator::generateElements (vector<Vertex*> lastVLine, vector<Vertex*> currVLine) {
    for(int i = 0; i < lastVLine.size() - 1; i++) {
        auto v1 = lastVLine[i];
        auto v2 = currVLine[i];
        auto v3 = currVLine[i + 1];
        auto v4 = lastVLine[i + 1];
        auto element = new Quadrilateral({ v1, v2, v3, v4 });
        cMesh.addElement(element);
    }
}

Mesh MeshShapesGenerator::genMesh(std::vector<RShapes> shapeList
    , const QuadArea &area) {
    
    cBoundary = generateMesheableBoundary(area);

    cMesh.addVertices(cBoundary.south);
    cMesh.addVertices(cBoundary.east);
    cMesh.addVertices(cBoundary.north);
    cMesh.addVertices(cBoundary.west);

    for(auto shape : shapeList) {
        switch (shape)
        {
        case RShapes::POOO_0:
            gen_POOO_0();
            break;
        case RShapes::POOO_90:
            gen_POOO_90();
            break;
        case RShapes::POOO_180:
            gen_POOO_180();
            break;
        case RShapes::POOO_270:
            gen_POOO_270();
            break;
        case RShapes::PPOO_0:
            gen_PPOO_0();
            break;
        case RShapes::PPOO_90:
            gen_PPOO_90();
            break;
        case RShapes::PPOO_180:
            gen_PPOO_180();
            break;
        case RShapes::PPOO_270:
            gen_PPOO_270();
            break;
        case RShapes::POPO_0:
            gen_POPO_0();
            break;
        case RShapes::POPO_90:
            gen_POPO_90();
            break;
        case RShapes::POPP_0:
            gen_POPP_0();
            break;
        case RShapes::POPP_90:
            gen_POPP_90();
            break;
        case RShapes::POPP_180:
            gen_POPP_180();
            break;
        case RShapes::POPP_270:
            gen_POPP_270();
            break;
        case RShapes::PPPP:
            gen_PPPP();
            break;
        case RShapes::OOOO:
            gen_OOOO();
            break;
        default:
            break;
        }
    }

    return cMesh;
}

void MeshShapesGenerator::gen_POOO_0() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(sLine, wLine);
cMesh.addVertex(ref1Vert);

double sElementSize = getElementSize(
    ref1Vert,
    eLine[1],
    VertexLineDirection::Horizontal,
    sLine.size()
);
double wElementSize = getElementSize(
    ref1Vert,
    nLine[1],
    VertexLineDirection::Vertical,
    wLine.size()
);

VertexLineParams Line1Params { 
    ref1Vert,
    eLine[1],
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
generateElements(sLine, newSLine);

VertexLineParams Line2Params {
    ref1Vert,
    nLine[1],
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
generateElements(wLine, newWLine);

cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east.erase(cBoundary.east.begin());
cBoundary.north.erase(cBoundary.north.begin());

}

void MeshShapesGenerator::gen_POOO_90() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(sLine, eLine);
cMesh.addVertex(ref1Vert);

double sElementSize = getElementSize(
    ref1Vert,
    wLine[1],
    VertexLineDirection::Horizontal,
    sLine.size()
);

double eElementSize = getElementSize(
    ref1Vert,
    nLine[nLine.size() - 2],
    VertexLineDirection::Vertical,
    eLine.size()
);

VertexLineParams Line1Params { 
    wLine[1],
    ref1Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
generateElements(sLine, newSLine);

VertexLineParams Line2Params {
    ref1Vert,
    nLine[nLine.size() - 2],
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
generateElements(eLine, newELine);

cBoundary.west.erase(cBoundary.west.begin());
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north.erase(cBoundary.north.end() - 1);

}

void MeshShapesGenerator::gen_POOO_180() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(nLine, eLine);
cMesh.addVertex(ref1Vert);

double nElementSize = getElementSize(
    ref1Vert,
    wLine[wLine.size() - 2],
    VertexLineDirection::Horizontal,
    nLine.size()
);
double eElementSize = getElementSize(
    ref1Vert,
    sLine[sLine.size() - 2],
    VertexLineDirection::Vertical,
    eLine.size()
);

VertexLineParams Line1Params { 
    wLine[wLine.size() - 2],
    ref1Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line1Params, cMesh);
generateElements(nLine, newNLine);

VertexLineParams Line2Params {
    sLine[sLine.size() - 2],
    ref1Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
generateElements(eLine, newELine);

cBoundary.west.erase(cBoundary.west.end() - 1);
cBoundary.south.erase(cBoundary.south.end() - 1);
cBoundary.east = newELine;
cBoundary.north = newNLine;

}

void MeshShapesGenerator::gen_POOO_270() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(nLine, wLine);
cMesh.addVertex(ref1Vert);

double nElementSize = getElementSize(
    ref1Vert,
    eLine[eLine.size() - 2],
    VertexLineDirection::Horizontal,
    nLine.size()
);
double wElementSize = getElementSize(
    ref1Vert,
    sLine[1],
    VertexLineDirection::Vertical,
    wLine.size()
);

VertexLineParams Line1Params { 
    ref1Vert,
    eLine[eLine.size() - 2],
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line1Params, cMesh);
generateElements(nLine, newNLine);

VertexLineParams Line2Params {
    sLine[1],
    ref1Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
generateElements(wLine, newWLine);

cBoundary.east.erase(cBoundary.east.end() - 1);
cBoundary.south.erase(cBoundary.south.begin());
cBoundary.west = newWLine;
cBoundary.north = newNLine;

}

void MeshShapesGenerator::gen_PPOO_0() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(sLine, wLine);
cMesh.addVertex(ref1Vert);

auto ref2Vert = getRefVertex(sLine, eLine);
cMesh.addVertex(ref2Vert);

double sElementSize = getElementSize(
    ref1Vert,
    ref2Vert,
    VertexLineDirection::Horizontal,
    sLine.size()
);
double wElementSize = getElementSize(
    ref1Vert,
    nLine[1],
    VertexLineDirection::Vertical,
    wLine.size()
);
double eElementSize = getElementSize(
    ref2Vert,
    nLine[nLine.size() - 2],
    VertexLineDirection::Vertical,
    eLine.size()
);

VertexLineParams Line1Params { 
    ref1Vert,
    ref2Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
generateElements(sLine, newSLine);

VertexLineParams Line2Params {
    ref1Vert,
    nLine[1],
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
generateElements(wLine, newWLine);

VertexLineParams Line3Params {
    ref2Vert,
    nLine[nLine.size() - 2],
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line3Params, cMesh);
generateElements(eLine, newELine);

cBoundary.east = newELine;
cBoundary.south = newSLine;
cBoundary.west = newWLine;
cBoundary.north.erase(cBoundary.north.begin());
cBoundary.north.erase(cBoundary.north.end() - 1);
}

void MeshShapesGenerator::gen_PPOO_90() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(sLine, eLine);
cMesh.addVertex(ref1Vert);

auto ref2Vert = getRefVertex(nLine, eLine);
cMesh.addVertex(ref2Vert);

double eElementSize = getElementSize(
    ref1Vert,
    ref2Vert,
    VertexLineDirection::Vertical,
    eLine.size()
);
double sElementSize = getElementSize(
    ref1Vert,
    wLine[1],
    VertexLineDirection::Horizontal,
    sLine.size()
);
double nElementSize = getElementSize(
    ref2Vert,
    wLine[wLine.size() - 2],
    VertexLineDirection::Horizontal,
    nLine.size()
);

VertexLineParams Line1Params { 
    ref1Vert,
    ref2Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line1Params, cMesh);
generateElements(eLine, newELine);

VertexLineParams Line2Params {
    wLine[1],
    ref1Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line2Params, cMesh);
generateElements(sLine, newSLine);

VertexLineParams Line3Params {
    wLine[wLine.size() - 2],
    ref2Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line3Params, cMesh);
generateElements(nLine, newNLine);

cBoundary.east = newELine;
cBoundary.south = newSLine;
cBoundary.west.erase(cBoundary.west.begin());
cBoundary.west.erase(cBoundary.west.end() - 1);
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_PPOO_180() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(nLine, eLine);
cMesh.addVertex(ref1Vert);

auto ref2Vert = getRefVertex(nLine, wLine);
cMesh.addVertex(ref2Vert);

double nElementSize = getElementSize(
    ref1Vert,
    ref2Vert,
    VertexLineDirection::Horizontal,
    nLine.size()
);
double wElementSize = getElementSize(
    ref2Vert,
    sLine[1],
    VertexLineDirection::Vertical,
    wLine.size()
);
double eElementSize = getElementSize(
    ref1Vert,
    sLine[sLine.size() - 2],
    VertexLineDirection::Vertical,
    eLine.size()
);

VertexLineParams Line1Params { 
    ref2Vert,
    ref1Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line1Params, cMesh);
generateElements(nLine, newNLine);

VertexLineParams Line2Params {
    sLine[sLine.size() - 2],
    ref1Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
generateElements(eLine, newELine);

VertexLineParams Line3Params {
    sLine[1],
    ref2Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line3Params, cMesh);
generateElements(wLine, newWLine);

cBoundary.east = newELine;
cBoundary.north = newNLine;
cBoundary.west = newWLine;
cBoundary.south.erase(cBoundary.south.begin());
cBoundary.south.erase(cBoundary.south.end() - 1);
}

void MeshShapesGenerator::gen_PPOO_270() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(nLine, wLine);
cMesh.addVertex(ref1Vert);

auto ref2Vert = getRefVertex(sLine, wLine);
cMesh.addVertex(ref2Vert);

double wElementSize = getElementSize(
    ref1Vert,
    ref2Vert,
    VertexLineDirection::Vertical,
    wLine.size()
);
double sElementSize = getElementSize(
    ref2Vert,
    eLine[1],
    VertexLineDirection::Horizontal,
    sLine.size()
);
double nElementSize = getElementSize(
    ref1Vert,
    eLine[eLine.size() - 2],
    VertexLineDirection::Horizontal,
    nLine.size()
);

VertexLineParams Line1Params { 
    ref2Vert,
    ref1Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line1Params, cMesh);
generateElements(wLine, newWLine);

VertexLineParams Line2Params {
    ref1Vert,
    eLine[eLine.size() - 2],
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line2Params, cMesh);
generateElements(nLine, newNLine);

VertexLineParams Line3Params {
    ref2Vert,
    eLine[1],
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line3Params, cMesh);
generateElements(sLine, newSLine);

cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east.erase(cBoundary.east.begin());
cBoundary.east.erase(cBoundary.east.end() - 1);
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_POPO_0() {
    gen_POOO_0();
    gen_POOO_180();
}

void MeshShapesGenerator::gen_POPO_90() {
    gen_POOO_90();
    gen_POOO_270();
}

void MeshShapesGenerator::gen_POPP_0() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(sLine, eLine);
cMesh.addVertex(ref1Vert);

auto ref2Vert = getRefVertex(sLine, wLine);
cMesh.addVertex(ref2Vert);

auto ref3Vert = getRefVertex(nLine, eLine);
cMesh.addVertex(ref3Vert);

double sElementSize = getElementSize(
    ref2Vert,
    ref1Vert,
    VertexLineDirection::Horizontal,
    sLine.size() + 1
);
double eElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Vertical,
    eLine.size() + 1
);
double wElementSize = getElementSize(
    ref2Vert,
    nLine[1],
    VertexLineDirection::Vertical,
    wLine.size()
);

VertexLineParams Line1Params { 
    ref1Vert,
    ref3Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line1Params, cMesh);
generateElements(eLine, newELine);

VertexLineParams Line2Params {
    ref2Vert,
    ref1Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line2Params, cMesh);
generateElements(sLine, newSLine);

VertexLineParams wLineParams {
    ref2Vert,
    nLine[1],
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(wLineParams, cMesh);
generateElements(wLine, newWLine);

auto rVertex = newWLine[newWLine.size() - 2];
double nElementSize = getElementSize(
    ref3Vert,
    rVertex,
    VertexLineDirection::Horizontal,
    nLine.size()
);
VertexLineParams nLineParams {
    rVertex,
    ref3Vert,
    nLine.size() - 1,
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(nLineParams, cMesh);
nLine.erase(nLine.begin());
generateElements(nLine, newNLine);

newWLine.erase(newWLine.end() - 1);
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;

}

void MeshShapesGenerator::gen_POPP_90() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(nLine, eLine);
cMesh.addVertex(ref1Vert);

auto ref2Vert = getRefVertex(sLine, eLine);
cMesh.addVertex(ref2Vert);

auto ref3Vert = getRefVertex(nLine, wLine);
cMesh.addVertex(ref3Vert);

double sElementSize = getElementSize(
    ref2Vert,
    wLine[1],
    VertexLineDirection::Horizontal,
    sLine.size()
);
double eElementSize = getElementSize(
    ref2Vert,
    ref1Vert,
    VertexLineDirection::Vertical,
    eLine.size() + 1
);
double nElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Horizontal,
    nLine.size() + 1
);

VertexLineParams Line1Params { 
    ref3Vert,
    ref1Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line1Params, cMesh);
generateElements(nLine, newNLine);

VertexLineParams Line2Params {
    ref2Vert,
    ref1Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
generateElements(eLine, newELine);

VertexLineParams Line3Params {
    wLine[1],
    ref2Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line3Params, cMesh);
generateElements(sLine, newSLine);

auto rVertex = newSLine[1];
double wElementSize = getElementSize(
    ref3Vert,
    rVertex,
    VertexLineDirection::Vertical,
    wLine.size()
);
VertexLineParams Line4Params {
    rVertex,
    ref3Vert,
    wLine.size() - 1,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line4Params, cMesh);
wLine.erase(wLine.begin());
generateElements(wLine, newWLine);

newSLine.erase(newSLine.end() - 1);
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_POPP_180() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(nLine, wLine);
cMesh.addVertex(ref1Vert);

auto ref2Vert = getRefVertex(nLine, eLine);
cMesh.addVertex(ref2Vert);

auto ref3Vert = getRefVertex(sLine, wLine);
cMesh.addVertex(ref3Vert);

double eElementSize = getElementSize(
    ref2Vert,
    sLine[sLine.size() - 2],
    VertexLineDirection::Vertical,
    eLine.size()
);
double nElementSize = getElementSize(
    ref2Vert,
    ref1Vert,
    VertexLineDirection::Horizontal,
    nLine.size() + 1
);
double wElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Vertical,
    wLine.size() + 1
);

VertexLineParams Line1Params { 
    ref3Vert,
    ref1Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line1Params, cMesh);
generateElements(wLine, newWLine);

VertexLineParams Line2Params {
    ref1Vert,
    ref2Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line2Params, cMesh);
generateElements(nLine, newNLine);

VertexLineParams Line3Params {
    sLine[sLine.size() - 2],
    ref2Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line3Params, cMesh);
generateElements(eLine, newELine);

auto rVertex = newELine[1];
double sElementSize = getElementSize(
    rVertex,
    ref3Vert,
    VertexLineDirection::Horizontal,
    sLine.size()
);
VertexLineParams Line4Params {
    ref3Vert,
    rVertex,
    sLine.size() - 1,
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line4Params, cMesh);
sLine.erase(sLine.end() - 1);
generateElements(sLine, newSLine);

newELine.erase(newELine.begin());
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_POPP_270() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(sLine, wLine);
cMesh.addVertex(ref1Vert);

auto ref2Vert = getRefVertex(nLine, wLine);
cMesh.addVertex(ref2Vert);

auto ref3Vert = getRefVertex(sLine, eLine);
cMesh.addVertex(ref3Vert);

double sElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Horizontal,
    sLine.size()
);
double nElementSize = getElementSize(
    eLine[eLine.size() - 2],
    ref2Vert,
    VertexLineDirection::Horizontal,
    nLine.size()
);
double wElementSize = getElementSize(
    ref2Vert,
    ref1Vert,
    VertexLineDirection::Vertical,
    wLine.size()
);

VertexLineParams Line1Params { 
    ref1Vert,
    ref3Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
generateElements(sLine, newSLine);

VertexLineParams Line2Params {
    ref1Vert,
    ref2Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
generateElements(wLine, newWLine);

VertexLineParams Line3Params {
    ref2Vert,
    eLine[eLine.size() - 2],
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line3Params, cMesh);
generateElements(nLine, newNLine);

auto rVertex = newNLine[newNLine.size() - 2];
double eElementSize = getElementSize(
    rVertex,
    ref3Vert,
    VertexLineDirection::Vertical,
    eLine.size() - 1
);
VertexLineParams Line4Params {
    ref3Vert,
    rVertex,
    eLine.size() - 1,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line4Params, cMesh);
eLine.erase(eLine.end() - 1);
generateElements(eLine, newELine);

newNLine.erase(newNLine.end() - 1);
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_PPPP() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(sLine, wLine);
cMesh.addVertex(ref1Vert);

auto ref2Vert = getRefVertex(sLine, eLine);
cMesh.addVertex(ref2Vert);

auto ref3Vert = getRefVertex(nLine, eLine);
cMesh.addVertex(ref3Vert);

auto ref4Vert = getRefVertex(nLine, wLine);
cMesh.addVertex(ref4Vert);

double sElementSize = getElementSize(
    ref2Vert,
    ref1Vert,
    VertexLineDirection::Horizontal,
    sLine.size() + 1
);
double eElementSize = getElementSize(
    ref3Vert,
    ref2Vert,
    VertexLineDirection::Vertical,
    eLine.size() + 1
);
double nElementSize = getElementSize(
    ref3Vert,
    ref4Vert,
    VertexLineDirection::Horizontal,
    nLine.size() + 1
);
double wElementSize = getElementSize(
    ref4Vert,
    ref1Vert,
    VertexLineDirection::Vertical,
    wLine.size() + 1
);

VertexLineParams Line1Params { 
    ref1Vert,
    ref2Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
generateElements(sLine, newSLine);

VertexLineParams Line2Params {
    ref2Vert,
    ref3Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
generateElements(eLine, newELine);

VertexLineParams Line3Params {
    ref4Vert,
    ref3Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line3Params, cMesh);
generateElements(nLine, newNLine);

VertexLineParams Line4Params {
    ref1Vert,
    ref4Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line4Params, cMesh);
generateElements(wLine, newWLine);

cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_OOOO() {

if(cBoundary.south.size() != cBoundary.north.size()
    || cBoundary.east.size() != cBoundary.west.size()) {
    throw std::invalid_argument("OOOO_TYPE: only equal sided elements quantity permitted.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto nLine = cBoundary.north;
auto eLine = cBoundary.east;

auto lastVLine = wLine; // last vertical line
vector<Vertex*> currVLine;
for(int xInd = 1; xInd < sLine.size() - 1; xInd++) {
    for(int yInd = 1; yInd < wLine.size() - 1; yInd++) {
        if (yInd == 1)
            currVLine.push_back(sLine[xInd]);
        double initXPos = sLine[xInd]->x;
        double finalXPos = nLine[xInd]->x;
        double initYPos = wLine[yInd]->y;
        double finalYPos = eLine[yInd]->y;
        double cXPos = initXPos + yInd * (finalXPos - initXPos) / (wLine.size() - 1);
        double cYPos = initYPos + xInd * (finalYPos - initYPos) / (sLine.size() - 1);
        auto v = new Vertex(cXPos, cYPos, 0);
        cMesh.addVertex(v);
        currVLine.push_back(v);

        if (yInd == wLine.size() - 2) {
            currVLine.push_back(nLine[xInd]);
            generateElements(lastVLine, currVLine);
            lastVLine = currVLine;
            currVLine.clear();
        }
    }
}

currVLine = eLine;
generateElements(lastVLine, currVLine);

}

} // namespace meshlib