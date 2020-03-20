#include "MeshShapesGenerator.hpp"

#include <stdexcept>

#include "meshlib.hpp"

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
        mesh.vertices.push_back(v);
        line.push_back(v);
    }
    line.push_back(params.end);

    return line;
}

double getElementSize(const VertexLine &line, VertexLineDirection direction, std::size_t qtdElements) {
    double Vertex::*coord = direction == VertexLineDirection::Horizontal ? &Vertex::x : &Vertex::y;
    double length = line[line.size() - 1]->*coord - line[0]->*coord;
    return length / qtdElements;
}

struct RefVertexParams {
    VertexLine hLine;
    double hLineELemSize;
    VertexLine vLine;
    double vLineElemSize;
};

Vertex* getRefVertex(RefVertexParams params) {
    auto hLFirst = *params.hLine.front();
    auto hLLast = *params.hLine.back();
    auto vLFirst = *params.vLine.front();
    auto vLLast = *params.vLine.back();

    double xPos = 0.0;
    double yPos = 0.0;

    if (hLFirst.Point::operator==(vLFirst)) {
        xPos = hLFirst.x + params.hLineELemSize;
        yPos = hLFirst.y + params.vLineElemSize;
    } else if (hLFirst.Point::operator==(vLLast)) {
        xPos = hLFirst.x + params.hLineELemSize;
        yPos = hLFirst.y - params.vLineElemSize;
    } else if (hLLast.Point::operator==(vLFirst)) {
        xPos = hLLast.x - params.hLineELemSize;
        yPos = hLLast.y + params.vLineElemSize;
    } else {
        xPos = hLLast.x - params.hLineELemSize;
        yPos = hLLast.y - params.vLineElemSize;
    }

    auto vertex = new Vertex(xPos, yPos, 0.0);
    return vertex;
}

void generateElements (vector<Vertex*> lastVLine, vector<Vertex*> currVLine, Mesh &mesh) {
    for(int i = 0; i < lastVLine.size() - 1; i++) {
        auto v1 = lastVLine[i];
        auto v2 = currVLine[i];
        auto v3 = currVLine[i + 1];
        auto v4 = lastVLine[i + 1];
        auto element = new Quadrilateral({ v1, v2, v3, v4 });
        mesh.elements.push_back(element);
    }
}

void addVerticesToMesh (vector<Vertex*> vertices, Mesh &mesh) {
    for(auto v : vertices) mesh.vertices.push_back(v);
}

Mesh MeshShapesGenerator::genMesh(std::vector<RShapes> shapeList
    , const AreaMesh &area) {

    cBoundary = generateMesheableBoundary(area);

    addVerticesToMesh(cBoundary.south, cMesh);
    addVerticesToMesh(cBoundary.east, cMesh);
    addVerticesToMesh(cBoundary.north, cMesh);
    addVerticesToMesh(cBoundary.west, cMesh);

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

double sElementSize = getElementSize(
    sLine,
    VertexLineDirection::Horizontal,
    sLine.size()
);
double wElementSize = getElementSize(
    wLine,
    VertexLineDirection::Vertical,
    wLine.size()
);

RefVertexParams r1Params {
    sLine,
    sElementSize,
    wLine,
    wElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

VertexLineParams Line1Params { 
    ref1Vert,
    eLine[1],
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
generateElements(sLine, newSLine, cMesh);

VertexLineParams Line2Params {
    ref1Vert,
    nLine[1],
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
generateElements(wLine, newWLine, cMesh);

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

double sElementSize = getElementSize(
    sLine,
    VertexLineDirection::Horizontal,
    sLine.size()
);
double eElementSize = getElementSize(
    eLine,
    VertexLineDirection::Vertical,
    eLine.size()
);

RefVertexParams r1Params {
    sLine,
    sElementSize,
    eLine,
    eElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

VertexLineParams Line1Params { 
    wLine[1],
    ref1Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
generateElements(sLine, newSLine, cMesh);

VertexLineParams Line2Params {
    ref1Vert,
    nLine[nLine.size() - 2],
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
generateElements(eLine, newELine, cMesh);

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

double nElementSize = getElementSize(
    nLine,
    VertexLineDirection::Horizontal,
    nLine.size()
);
double eElementSize = getElementSize(
    eLine,
    VertexLineDirection::Vertical,
    eLine.size()
);

RefVertexParams r1Params {
    nLine,
    nElementSize,
    eLine,
    eElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

VertexLineParams Line1Params { 
    wLine[wLine.size() - 2],
    ref1Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line1Params, cMesh);
generateElements(nLine, newNLine, cMesh);

VertexLineParams Line2Params {
    sLine[sLine.size() - 2],
    ref1Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
generateElements(eLine, newELine, cMesh);

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

double nElementSize = getElementSize(
    nLine,
    VertexLineDirection::Horizontal,
    nLine.size()
);
double wElementSize = getElementSize(
    wLine,
    VertexLineDirection::Vertical,
    wLine.size()
);

RefVertexParams r1Params {
    nLine,
    nElementSize,
    wLine,
    wElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

VertexLineParams Line1Params { 
    ref1Vert,
    eLine[eLine.size() - 2],
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line1Params, cMesh);
generateElements(nLine, newNLine, cMesh);

VertexLineParams Line2Params {
    sLine[1],
    ref1Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
generateElements(wLine, newWLine, cMesh);

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

double sElementSize = getElementSize(
    sLine,
    VertexLineDirection::Horizontal,
    sLine.size() + 1
);
double wElementSize = getElementSize(
    wLine,
    VertexLineDirection::Vertical,
    wLine.size()
);
double eElementSize = getElementSize(
    eLine,
    VertexLineDirection::Vertical,
    eLine.size()
);

RefVertexParams r1Params {
    sLine,
    sElementSize,
    wLine,
    wElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

RefVertexParams r2Params {
    sLine,
    sElementSize,
    eLine,
    eElementSize
};
auto ref2Vert = getRefVertex(r2Params);
cMesh.vertices.push_back(ref2Vert);

VertexLineParams Line1Params { 
    ref1Vert,
    ref2Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
generateElements(sLine, newSLine, cMesh);

VertexLineParams Line2Params {
    ref1Vert,
    nLine[1],
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
generateElements(wLine, newWLine, cMesh);

VertexLineParams Line3Params {
    ref2Vert,
    nLine[nLine.size() - 2],
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line3Params, cMesh);
generateElements(eLine, newELine, cMesh);

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

double eElementSize = getElementSize(
    eLine,
    VertexLineDirection::Vertical,
    eLine.size() + 1
);
double sElementSize = getElementSize(
    sLine,
    VertexLineDirection::Horizontal,
    sLine.size()
);
double nElementSize = getElementSize(
    nLine,
    VertexLineDirection::Horizontal,
    nLine.size()
);

RefVertexParams r1Params {
    sLine,
    sElementSize,
    eLine,
    eElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

RefVertexParams r2Params {
    nLine,
    nElementSize,
    eLine,
    eElementSize
};
auto ref2Vert = getRefVertex(r2Params);
cMesh.vertices.push_back(ref2Vert);

VertexLineParams Line1Params { 
    ref1Vert,
    ref2Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line1Params, cMesh);
generateElements(eLine, newELine, cMesh);

VertexLineParams Line2Params {
    wLine[1],
    ref1Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line2Params, cMesh);
generateElements(sLine, newSLine, cMesh);

VertexLineParams Line3Params {
    wLine[wLine.size() - 2],
    ref2Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line3Params, cMesh);
generateElements(nLine, newNLine, cMesh);

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

double nElementSize = getElementSize(
    nLine,
    VertexLineDirection::Horizontal,
    nLine.size() + 1
);
double wElementSize = getElementSize(
    wLine,
    VertexLineDirection::Vertical,
    wLine.size()
);
double eElementSize = getElementSize(
    eLine,
    VertexLineDirection::Vertical,
    eLine.size()
);

RefVertexParams r1Params {
    nLine,
    nElementSize,
    eLine,
    eElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

RefVertexParams r2Params {
    nLine,
    nElementSize,
    wLine,
    wElementSize
};
auto ref2Vert = getRefVertex(r2Params);
cMesh.vertices.push_back(ref2Vert);

VertexLineParams Line1Params { 
    ref2Vert,
    ref1Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line1Params, cMesh);
generateElements(nLine, newNLine, cMesh);

VertexLineParams Line2Params {
    sLine[sLine.size() - 2],
    ref1Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
generateElements(eLine, newELine, cMesh);

VertexLineParams Line3Params {
    sLine[1],
    ref2Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line3Params, cMesh);
generateElements(wLine, newWLine, cMesh);

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

double wElementSize = getElementSize(
    wLine,
    VertexLineDirection::Vertical,
    wLine.size() + 1
);
double sElementSize = getElementSize(
    sLine,
    VertexLineDirection::Horizontal,
    sLine.size()
);
double nElementSize = getElementSize(
    nLine,
    VertexLineDirection::Horizontal,
    nLine.size()
);

RefVertexParams r1Params {
    nLine,
    nElementSize,
    wLine,
    wElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

RefVertexParams r2Params {
    sLine,
    sElementSize,
    wLine,
    wElementSize
};
auto ref2Vert = getRefVertex(r2Params);
cMesh.vertices.push_back(ref2Vert);

VertexLineParams Line1Params { 
    ref2Vert,
    ref1Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line1Params, cMesh);
generateElements(wLine, newWLine, cMesh);

VertexLineParams Line2Params {
    ref1Vert,
    eLine[eLine.size() - 2],
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line2Params, cMesh);
generateElements(nLine, newNLine, cMesh);

VertexLineParams Line3Params {
    ref2Vert,
    eLine[1],
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line3Params, cMesh);
generateElements(sLine, newSLine, cMesh);

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

double sElementSize = getElementSize(
    sLine,
    VertexLineDirection::Horizontal,
    sLine.size() + 1
);
double eElementSize = getElementSize(
    eLine,
    VertexLineDirection::Vertical,
    eLine.size() + 1
);
double nElementSize = getElementSize(
    nLine,
    VertexLineDirection::Horizontal,
    nLine.size()
);
double wElementSize = getElementSize(
    wLine,
    VertexLineDirection::Vertical,
    wLine.size()
);

RefVertexParams r1Params {
    sLine,
    sElementSize,
    eLine,
    eElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

RefVertexParams r2Params {
    sLine,
    sElementSize,
    wLine,
    wElementSize
};
auto ref2Vert = getRefVertex(r2Params);
cMesh.vertices.push_back(ref2Vert);

RefVertexParams r3Params {
    nLine,
    nElementSize,
    eLine,
    eElementSize
};
auto ref3Vert = getRefVertex(r3Params);
cMesh.vertices.push_back(ref3Vert);

VertexLineParams Line1Params { 
    ref1Vert,
    ref3Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line1Params, cMesh);
generateElements(eLine, newELine, cMesh);

VertexLineParams Line2Params {
    ref2Vert,
    ref1Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line2Params, cMesh);
generateElements(sLine, newSLine, cMesh);

VertexLineParams wLineParams {
    ref2Vert,
    nLine[1],
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(wLineParams, cMesh);
generateElements(wLine, newWLine, cMesh);

auto rVertex = newWLine[newWLine.size() - 2];
VertexLineParams nLineParams {
    rVertex,
    ref3Vert,
    nLine.size() - 1,
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(nLineParams, cMesh);
nLine.erase(nLine.begin());
generateElements(nLine, newNLine, cMesh);

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

double sElementSize = getElementSize(
    sLine,
    VertexLineDirection::Horizontal,
    sLine.size()
);
double eElementSize = getElementSize(
    eLine,
    VertexLineDirection::Vertical,
    eLine.size() + 1
);
double nElementSize = getElementSize(
    nLine,
    VertexLineDirection::Horizontal,
    nLine.size() + 1
);
double wElementSize = getElementSize(
    wLine,
    VertexLineDirection::Vertical,
    wLine.size()
);

RefVertexParams r1Params {
    nLine,
    nElementSize,
    eLine,
    eElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

RefVertexParams r2Params {
    sLine,
    sElementSize,
    eLine,
    eElementSize
};
auto ref2Vert = getRefVertex(r2Params);
cMesh.vertices.push_back(ref2Vert);

RefVertexParams r3Params {
    nLine,
    nElementSize,
    wLine,
    wElementSize
};
auto ref3Vert = getRefVertex(r3Params);
cMesh.vertices.push_back(ref3Vert);

VertexLineParams Line1Params { 
    ref3Vert,
    ref1Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line1Params, cMesh);
generateElements(nLine, newNLine, cMesh);

VertexLineParams Line2Params {
    ref2Vert,
    ref1Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
generateElements(eLine, newELine, cMesh);

VertexLineParams Line3Params {
    wLine[1],
    ref2Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line3Params, cMesh);
generateElements(sLine, newSLine, cMesh);

auto rVertex = newSLine[1];
VertexLineParams Line4Params {
    rVertex,
    ref3Vert,
    wLine.size() - 1,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line4Params, cMesh);
wLine.erase(wLine.begin());
generateElements(wLine, newWLine, cMesh);

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

double sElementSize = getElementSize(
    sLine,
    VertexLineDirection::Horizontal,
    sLine.size()
);
double eElementSize = getElementSize(
    eLine,
    VertexLineDirection::Vertical,
    eLine.size()
);
double nElementSize = getElementSize(
    nLine,
    VertexLineDirection::Horizontal,
    nLine.size() + 1
);
double wElementSize = getElementSize(
    wLine,
    VertexLineDirection::Vertical,
    wLine.size() + 1
);

RefVertexParams r1Params {
    nLine,
    nElementSize,
    wLine,
    wElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

RefVertexParams r2Params {
    nLine,
    nElementSize,
    eLine,
    eElementSize
};
auto ref2Vert = getRefVertex(r2Params);
cMesh.vertices.push_back(ref2Vert);

RefVertexParams r3Params {
    sLine,
    sElementSize,
    wLine,
    wElementSize
};
auto ref3Vert = getRefVertex(r3Params);
cMesh.vertices.push_back(ref3Vert);

VertexLineParams Line1Params { 
    ref3Vert,
    ref1Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line1Params, cMesh);
generateElements(wLine, newWLine, cMesh);

VertexLineParams Line2Params {
    ref1Vert,
    ref2Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line2Params, cMesh);
generateElements(nLine, newNLine, cMesh);

VertexLineParams Line3Params {
    sLine[sLine.size() - 2],
    ref2Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line3Params, cMesh);
generateElements(eLine, newELine, cMesh);

auto rVertex = newELine[1];
VertexLineParams Line4Params {
    ref3Vert,
    rVertex,
    sLine.size() - 1,
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line4Params, cMesh);
sLine.erase(sLine.end() - 1);
generateElements(sLine, newSLine, cMesh);

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

double sElementSize = getElementSize(
    sLine,
    VertexLineDirection::Horizontal,
    sLine.size() + 1
);
double eElementSize = getElementSize(
    eLine,
    VertexLineDirection::Vertical,
    eLine.size()
);
double nElementSize = getElementSize(
    nLine,
    VertexLineDirection::Horizontal,
    nLine.size()
);
double wElementSize = getElementSize(
    wLine,
    VertexLineDirection::Vertical,
    wLine.size() + 1
);

RefVertexParams r1Params {
    sLine,
    sElementSize,
    wLine,
    wElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

RefVertexParams r2Params {
    nLine,
    nElementSize,
    wLine,
    wElementSize
};
auto ref2Vert = getRefVertex(r2Params);
cMesh.vertices.push_back(ref2Vert);

RefVertexParams r3Params {
    sLine,
    sElementSize,
    eLine,
    eElementSize
};
auto ref3Vert = getRefVertex(r3Params);
cMesh.vertices.push_back(ref3Vert);

VertexLineParams Line1Params { 
    ref1Vert,
    ref3Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
generateElements(sLine, newSLine, cMesh);

VertexLineParams Line2Params {
    ref1Vert,
    ref2Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
generateElements(wLine, newWLine, cMesh);

VertexLineParams Line3Params {
    ref2Vert,
    eLine[eLine.size() - 2],
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line3Params, cMesh);
generateElements(nLine, newNLine, cMesh);

auto rVertex = newNLine[newNLine.size() - 2];
VertexLineParams Line4Params {
    ref3Vert,
    rVertex,
    eLine.size() - 1,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line4Params, cMesh);
eLine.erase(eLine.end() - 1);
generateElements(eLine, newELine, cMesh);

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

double sElementSize = getElementSize(
    sLine,
    VertexLineDirection::Horizontal,
    sLine.size() + 1
);
double eElementSize = getElementSize(
    eLine,
    VertexLineDirection::Vertical,
    eLine.size() + 1
);
double nElementSize = getElementSize(
    nLine,
    VertexLineDirection::Horizontal,
    nLine.size() + 1
);
double wElementSize = getElementSize(
    wLine,
    VertexLineDirection::Vertical,
    wLine.size() + 1
);

RefVertexParams r1Params {
    sLine,
    sElementSize,
    wLine,
    wElementSize
};
auto ref1Vert = getRefVertex(r1Params);
cMesh.vertices.push_back(ref1Vert);

RefVertexParams r2Params {
    sLine,
    sElementSize,
    eLine,
    eElementSize
};
auto ref2Vert = getRefVertex(r2Params);
cMesh.vertices.push_back(ref2Vert);

RefVertexParams r3Params {
    nLine,
    nElementSize,
    eLine,
    eElementSize
};
auto ref3Vert = getRefVertex(r3Params);
cMesh.vertices.push_back(ref3Vert);

RefVertexParams r4Params {
    nLine,
    nElementSize,
    wLine,
    wElementSize
};
auto ref4Vert = getRefVertex(r4Params);
cMesh.vertices.push_back(ref4Vert);

VertexLineParams Line1Params { 
    ref1Vert,
    ref2Vert,
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
generateElements(sLine, newSLine, cMesh);

VertexLineParams Line2Params {
    ref2Vert,
    ref3Vert,
    eLine.size(),
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
generateElements(eLine, newELine, cMesh);

VertexLineParams Line3Params {
    ref4Vert,
    ref3Vert,
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line3Params, cMesh);
generateElements(nLine, newNLine, cMesh);

VertexLineParams Line4Params {
    ref1Vert,
    ref4Vert,
    wLine.size(),
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line4Params, cMesh);
generateElements(wLine, newWLine, cMesh);

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
        cMesh.vertices.push_back(v);
        currVLine.push_back(v);

        if (yInd == wLine.size() - 2) {
            currVLine.push_back(nLine[xInd]);
            generateElements(lastVLine, currVLine, cMesh);
            lastVLine = currVLine;
            currVLine.clear();
        }
    }
}

currVLine = eLine;
generateElements(lastVLine, currVLine, cMesh);

}

} // namespace meshlib