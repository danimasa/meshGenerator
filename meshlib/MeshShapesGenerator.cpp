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

struct SimpleLine {
    Point* init_point;
    Point* final_point;
    
    SimpleLine(Point* ip, Point* fp)
        : init_point(ip), final_point(fp) {}
};

Vertex* findIntersection (const SimpleLine& l1, const SimpleLine& l2) {
    double yi1 = l1.init_point->y;
    double yi2 = l2.init_point->y;
    double xi1 = l1.init_point->x;
    double xi2 = l2.init_point->x;
    double yf1 = l1.final_point->y;
    double yf2 = l2.final_point->y;
    double xf1 = l1.final_point->x;
    double xf2 = l2.final_point->x;

    double a1 = yf1 - yi1;
    double b1 = xi1 - xf1;
    double c1 = a1 * xi1 + b1 * yi1;

    double a2 = yf2 - yi2;
    double b2 = xi2 - xf2;
    double c2 = a2 * xi2 + b2 * yi2;

    double determinant = a1*b2 - a2*b1;
    double x = (b2*c1 - b1*c2)/determinant;
    double y = (a1*c2 - a2*c1)/determinant;

    auto vertex = new Vertex(x, y, 0);
    return vertex;
}

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
        hLine.size() + 2
    );

    double vElementSize = getElementSize(
        vLine[0],
        vLine[vLine.size() - 1],
        VertexLineDirection::Vertical,
        vLine.size() + 2
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
        case RShapes::OOOO_C:
            gen_OOOO_C();
            break;
        case RShapes::AOOOP_0:
            gen_AOOOP_0();
            break;
        case RShapes::AOOOP_90:
            gen_AOOOP_90();
            break;
        case RShapes::AOOOP_180:
            gen_AOOOP_180();
            break;
        case RShapes::AOOOP_270:
            gen_AOOOP_270();
            break;
        case RShapes::OOOO_L_0:
            gen_OOOO_L_0();
            break;
        case RShapes::OOOO_L_90:
            gen_OOOO_L_90();
            break;
        case RShapes::OOOO_L_180:
            gen_OOOO_L_180();
            break;
        case RShapes::OOOO_L_270:
            gen_OOOO_L_270();
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

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(sLine, wLine);
cMesh.addVertex(ref1Vert);

auto ref2Vert = getRefVertex(nLine, eLine);
cMesh.addVertex(ref2Vert);

SimpleLine l1(ref1Vert, eLine[1]);
SimpleLine l2(ref2Vert, sLine[sLine.size() - 2]);
auto ref3Vert = findIntersection(l1, l2);
cMesh.addVertex(ref3Vert);

SimpleLine l3(ref1Vert, nLine[1]);
SimpleLine l4(ref2Vert, wLine[wLine.size() - 2]);
auto ref4Vert = findIntersection(l3, l4);
cMesh.addVertex(ref4Vert);

double sElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Horizontal,
    sLine.size() - 1
);
double eElementSize = getElementSize(
    ref3Vert,
    ref2Vert,
    VertexLineDirection::Vertical,
    eLine.size() - 1
);
double wElementSize = getElementSize(
    ref1Vert,
    ref4Vert,
    VertexLineDirection::Vertical,
    wLine.size() - 1
);
double nElementSize = getElementSize(
    ref4Vert,
    ref2Vert,
    VertexLineDirection::Horizontal,
    nLine.size() - 1
);

VertexLineParams Line1Params { 
    ref1Vert,
    ref3Vert,
    sLine.size() - 1,
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
newSLine.push_back(eLine[1]);
generateElements(sLine, newSLine);

VertexLineParams Line2Params {
    ref1Vert,
    ref4Vert,
    wLine.size() - 1,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
newWLine.push_back(nLine[1]);
generateElements(wLine, newWLine);

VertexLineParams Line3Params {
    ref3Vert,
    ref2Vert,
    eLine.size() - 1,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line3Params, cMesh);
eLine.erase(eLine.begin());
generateElements(eLine, newELine);

VertexLineParams Line4Params {
    ref4Vert,
    ref2Vert,
    nLine.size() - 1,
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line4Params, cMesh);
nLine.erase(nLine.begin());
generateElements(nLine, newNLine);

newWLine.erase(newWLine.end() - 1);
newSLine.erase(newSLine.end() - 1);
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_POPO_90() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(sLine, eLine);
cMesh.addVertex(ref1Vert);

auto ref2Vert = getRefVertex(nLine, wLine);
cMesh.addVertex(ref2Vert);

SimpleLine l1(ref1Vert, nLine[nLine.size() - 2]);
SimpleLine l2(ref2Vert, eLine[eLine.size() - 2]);
auto ref3Vert = findIntersection(l1, l2);
cMesh.addVertex(ref3Vert);

SimpleLine l3(ref1Vert, wLine[1]);
SimpleLine l4(ref2Vert, sLine[1]);
auto ref4Vert = findIntersection(l3, l4);
cMesh.addVertex(ref4Vert);

double sElementSize = getElementSize(
    ref4Vert,
    ref1Vert,
    VertexLineDirection::Horizontal,
    sLine.size() - 1
);
double eElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Vertical,
    eLine.size() - 1
);
double wElementSize = getElementSize(
    ref4Vert,
    ref2Vert,
    VertexLineDirection::Vertical,
    wLine.size() - 1
);
double nElementSize = getElementSize(
    ref2Vert,
    ref3Vert,
    VertexLineDirection::Horizontal,
    nLine.size() - 1
);

VertexLineParams Line1Params { 
    ref1Vert,
    ref3Vert,
    eLine.size() - 1,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line1Params, cMesh);
newELine.push_back(nLine[nLine.size() - 2]);
generateElements(eLine, newELine);

VertexLineParams Line2Params {
    ref4Vert,
    ref1Vert,
    sLine.size() - 1,
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line2Params, cMesh);
newSLine.insert(newSLine.begin(), wLine[1]);
generateElements(sLine, newSLine);

VertexLineParams Line3Params {
    ref2Vert,
    ref3Vert,
    nLine.size() - 1,
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line3Params, cMesh);
nLine.erase(nLine.end() - 1);
generateElements(nLine, newNLine);

VertexLineParams Line4Params {
    ref4Vert,
    ref2Vert,
    wLine.size() - 1,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line4Params, cMesh);
wLine.erase(wLine.begin());
generateElements(wLine, newWLine);

newSLine.erase(newSLine.begin());
newELine.erase(newELine.end() - 1);
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
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

SimpleLine l1(ref2Vert, nLine[1]);
SimpleLine l2(ref3Vert, wLine[wLine.size() - 2]);
auto ref4Vert = findIntersection(l1, l2);
cMesh.addVertex(ref4Vert);

double sElementSize = getElementSize(
    ref2Vert,
    ref1Vert,
    VertexLineDirection::Horizontal,
    sLine.size()
);
double eElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Vertical,
    eLine.size()
);
double wElementSize = getElementSize(
    ref2Vert,
    ref4Vert,
    VertexLineDirection::Vertical,
    wLine.size() - 1
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
    ref4Vert,
    wLine.size() - 1,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(wLineParams, cMesh);
newWLine.push_back(nLine[1]);
generateElements(wLine, newWLine);

double nElementSize = getElementSize(
    ref3Vert,
    ref4Vert,
    VertexLineDirection::Horizontal,
    nLine.size() - 1
);
VertexLineParams nLineParams {
    ref4Vert,
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

SimpleLine l1(ref2Vert, wLine[1]);
SimpleLine l2(ref3Vert, sLine[1]);
auto ref4Vert = findIntersection(l1, l2);
cMesh.addVertex(ref4Vert);

double sElementSize = getElementSize(
    ref2Vert,
    ref4Vert,
    VertexLineDirection::Horizontal,
    sLine.size() - 1
);
double eElementSize = getElementSize(
    ref2Vert,
    ref1Vert,
    VertexLineDirection::Vertical,
    eLine.size()
);
double nElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Horizontal,
    nLine.size()
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
    ref4Vert,
    ref2Vert,
    sLine.size() - 1,
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line3Params, cMesh);
newSLine.insert(newSLine.begin(), wLine[1]);
generateElements(sLine, newSLine);

double wElementSize = getElementSize(
    ref3Vert,
    ref4Vert,
    VertexLineDirection::Vertical,
    wLine.size() - 1
);
VertexLineParams Line4Params {
    ref4Vert,
    ref3Vert,
    wLine.size() - 1,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line4Params, cMesh);
wLine.erase(wLine.begin());
generateElements(wLine, newWLine);

newSLine.erase(newSLine.begin());
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

SimpleLine l1(ref2Vert, sLine[sLine.size() - 2]);
SimpleLine l2(ref3Vert, eLine[1]);
auto ref4Vert = findIntersection(l1, l2);
cMesh.addVertex(ref4Vert);

double eElementSize = getElementSize(
    ref2Vert,
    ref4Vert,
    VertexLineDirection::Vertical,
    eLine.size() - 1
);
double nElementSize = getElementSize(
    ref2Vert,
    ref1Vert,
    VertexLineDirection::Horizontal,
    nLine.size()
);
double wElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Vertical,
    wLine.size()
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
    ref4Vert,
    ref2Vert,
    eLine.size() - 1,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line3Params, cMesh);
newELine.insert(newELine.begin(), sLine[sLine.size() - 2]);
generateElements(eLine, newELine);

double sElementSize = getElementSize(
    ref4Vert,
    ref3Vert,
    VertexLineDirection::Horizontal,
    sLine.size() - 1
);
VertexLineParams Line4Params {
    ref3Vert,
    ref4Vert,
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

SimpleLine l1(ref2Vert, eLine[eLine.size() - 2]);
SimpleLine l2(ref3Vert, nLine[nLine.size() - 2]);
auto ref4Vert = findIntersection(l1, l2);
cMesh.addVertex(ref4Vert);

double sElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Horizontal,
    sLine.size()
);
double nElementSize = getElementSize(
    ref4Vert,
    ref2Vert,
    VertexLineDirection::Horizontal,
    nLine.size() - 1
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
    ref4Vert,
    nLine.size() - 1,
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line3Params, cMesh);
newNLine.push_back(eLine[eLine.size() - 2]);
generateElements(nLine, newNLine);

double eElementSize = getElementSize(
    ref4Vert,
    ref3Vert,
    VertexLineDirection::Vertical,
    eLine.size() - 1
);
VertexLineParams Line4Params {
    ref3Vert,
    ref4Vert,
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
    sLine.size()
);
double eElementSize = getElementSize(
    ref3Vert,
    ref2Vert,
    VertexLineDirection::Vertical,
    eLine.size()
);
double nElementSize = getElementSize(
    ref3Vert,
    ref4Vert,
    VertexLineDirection::Horizontal,
    nLine.size()
);
double wElementSize = getElementSize(
    ref4Vert,
    ref1Vert,
    VertexLineDirection::Vertical,
    wLine.size()
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

void MeshShapesGenerator::gen_OOOO_C() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

double sElementSize = getElementSize(
    eLine[1],
    wLine[1],
    VertexLineDirection::Horizontal,
    sLine.size()
);
double eElementSize = getElementSize(
    nLine[nLine.size() - 2],
    sLine[sLine.size() - 2],
    VertexLineDirection::Vertical,
    eLine.size()
);
double nElementSize = getElementSize(
    eLine[eLine.size() - 2],
    wLine[wLine.size() - 2],
    VertexLineDirection::Horizontal,
    nLine.size()
);
double wElementSize = getElementSize(
    nLine[1],
    sLine[1],
    VertexLineDirection::Vertical,
    wLine.size()
);

VertexLineParams Line1Params { 
    wLine[1],
    eLine[1],
    sLine.size(),
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
generateElements(sLine, newSLine);

VertexLineParams Line2Params {
    wLine[wLine.size() - 2],
    eLine[eLine.size() - 2],
    nLine.size(),
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line2Params, cMesh);
generateElements(nLine, newNLine);

VertexLineParams Line3Params {
    newSLine[newSLine.size() - 2],
    newNLine[newNLine.size() - 2],
    eLine.size() - 2,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line3Params, cMesh);
eLine.erase(eLine.end() - 1);
eLine.erase(eLine.begin());
generateElements(eLine, newELine);

VertexLineParams Line4Params {
    newSLine[1],
    newNLine[1],
    wLine.size() - 2,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line4Params, cMesh);
wLine.erase(wLine.end() - 1);
wLine.erase(wLine.begin());
generateElements(wLine, newWLine);

newNLine.erase(newNLine.end() - 1);
newNLine.erase(newNLine.begin());
newSLine.erase(newSLine.end() - 1);
newSLine.erase(newSLine.begin());
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_AOOOP_0() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(sLine, wLine);
cMesh.addVertex(ref1Vert);

SimpleLine l1(sLine[sLine.size() - 2], nLine[nLine.size() - 2]);
SimpleLine l2(wLine[wLine.size() - 2], eLine[eLine.size() - 2]);
auto ref2Vert = findIntersection(l1, l2);
cMesh.addVertex(ref2Vert);

SimpleLine l3(ref1Vert, eLine[1]);
auto ref3Vert = findIntersection(l3, l1);
cMesh.addVertex(ref3Vert);

SimpleLine l4(ref1Vert, nLine[1]);
auto ref4Vert = findIntersection(l4, l2);
cMesh.addVertex(ref4Vert);

double sElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Horizontal,
    sLine.size() - 1
);
double wElementSize = getElementSize(
    ref1Vert,
    ref4Vert,
    VertexLineDirection::Vertical,
    wLine.size() - 1
);
double nElementSize = getElementSize(
    ref4Vert,
    ref2Vert,
    VertexLineDirection::Horizontal,
    nLine.size() - 2
);
double eElementSize = getElementSize(
    ref3Vert,
    ref2Vert,
    VertexLineDirection::Vertical,
    eLine.size() - 2
);

VertexLineParams Line1Params { 
    ref1Vert,
    ref3Vert,
    sLine.size() - 1,
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
newSLine.push_back(eLine[1]);
generateElements(sLine, newSLine);

VertexLineParams Line2Params {
    ref3Vert,
    ref2Vert,
    eLine.size() - 2,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
eLine.erase(eLine.begin());
newELine.push_back(nLine[nLine.size() - 2]);
generateElements(eLine, newELine);

VertexLineParams Line3Params {
    ref4Vert,
    ref2Vert,
    nLine.size() - 2,
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line3Params, cMesh);
nLine.erase(nLine.end() - 1);
newNLine.insert(newNLine.begin(), wLine[wLine.size() - 2]);
generateElements(nLine, newNLine);

VertexLineParams Line4Params {
    ref1Vert,
    ref4Vert,
    wLine.size() - 1,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line4Params, cMesh);
newWLine.push_back(nLine[1]);
generateElements(wLine, newWLine);

newSLine.erase(newSLine.end() - 1);
newELine.erase(newELine.end() - 1);
newNLine.erase(newNLine.begin());
newWLine.erase(newWLine.end() - 1);
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_AOOOP_90() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(sLine, eLine);
cMesh.addVertex(ref1Vert);

SimpleLine l1(sLine[1], nLine[1]);
SimpleLine l2(wLine[wLine.size() - 2], eLine[eLine.size() - 2]);
auto ref2Vert = findIntersection(l1, l2);
cMesh.addVertex(ref2Vert);

SimpleLine l3(ref1Vert, nLine[nLine.size() - 2]);
auto ref3Vert = findIntersection(l3, l2);
cMesh.addVertex(ref3Vert);

SimpleLine l4(ref1Vert, wLine[1]);
auto ref4Vert = findIntersection(l4, l1);
cMesh.addVertex(ref4Vert);

double sElementSize = getElementSize(
    ref4Vert,
    ref1Vert,
    VertexLineDirection::Horizontal,
    sLine.size() - 1
);
double wElementSize = getElementSize(
    ref4Vert,
    ref2Vert,
    VertexLineDirection::Vertical,
    wLine.size() - 2
);
double nElementSize = getElementSize(
    ref2Vert,
    ref3Vert,
    VertexLineDirection::Horizontal,
    nLine.size() - 2
);
double eElementSize = getElementSize(
    ref1Vert,
    ref3Vert,
    VertexLineDirection::Vertical,
    eLine.size() - 1
);

VertexLineParams Line1Params { 
    ref1Vert,
    ref3Vert,
    eLine.size() - 1,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line1Params, cMesh);
newELine.push_back(nLine[nLine.size() - 2]);
generateElements(eLine, newELine);

VertexLineParams Line2Params {
    ref2Vert,
    ref3Vert,
    nLine.size() - 2,
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line2Params, cMesh);
nLine.erase(nLine.end() - 1);
newNLine.insert(newNLine.begin(), wLine[wLine.size() - 2]);
generateElements(nLine, newNLine);

VertexLineParams Line3Params {
    ref4Vert,
    ref2Vert,
    wLine.size() - 2,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line3Params, cMesh);
wLine.erase(wLine.end() - 1);
newWLine.insert(newWLine.begin(), sLine[1]);
generateElements(wLine, newWLine);

VertexLineParams Line4Params {
    ref4Vert,
    ref1Vert,
    sLine.size() - 1,
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line4Params, cMesh);
newSLine.insert(newSLine.begin(), wLine[1]);
generateElements(sLine, newSLine);

newSLine.erase(newSLine.begin());
newELine.erase(newELine.end() - 1);
newNLine.erase(newNLine.begin());
newWLine.erase(newWLine.begin());
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_AOOOP_180() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(nLine, eLine);
cMesh.addVertex(ref1Vert);

SimpleLine l1(wLine[1], eLine[1]);
SimpleLine l2(sLine[1], nLine[1]);
auto ref2Vert = findIntersection(l1, l2);
cMesh.addVertex(ref2Vert);

SimpleLine l3(ref1Vert, wLine[wLine.size() - 2]);
auto ref3Vert = findIntersection(l3, l2);
cMesh.addVertex(ref3Vert);

SimpleLine l4(ref1Vert, sLine[sLine.size() - 2]);
auto ref4Vert = findIntersection(l4, l1);
cMesh.addVertex(ref4Vert);

double sElementSize = getElementSize(
    ref2Vert,
    ref4Vert,
    VertexLineDirection::Horizontal,
    sLine.size() - 2
);
double wElementSize = getElementSize(
    ref2Vert,
    ref3Vert,
    VertexLineDirection::Vertical,
    wLine.size() - 2
);
double nElementSize = getElementSize(
    ref3Vert,
    ref1Vert,
    VertexLineDirection::Horizontal,
    nLine.size() - 1
);
double eElementSize = getElementSize(
    ref4Vert,
    ref1Vert,
    VertexLineDirection::Vertical,
    eLine.size() - 1
);

VertexLineParams Line1Params { 
    ref3Vert,
    ref1Vert,
    nLine.size() - 1,
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line1Params, cMesh);
newNLine.insert(newNLine.begin(), wLine[wLine.size() - 2]);
generateElements(nLine, newNLine);

VertexLineParams Line2Params {
    ref2Vert,
    ref3Vert,
    wLine.size() - 2,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
wLine.erase(wLine.end() - 1);
newWLine.insert(newWLine.begin(), sLine[1]);
generateElements(wLine, newWLine);

VertexLineParams Line3Params {
    ref2Vert,
    ref4Vert,
    sLine.size() - 2,
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line3Params, cMesh);
sLine.erase(sLine.begin());
newSLine.push_back(eLine[1]);
generateElements(sLine, newSLine);

VertexLineParams Line4Params {
    ref4Vert,
    ref1Vert,
    eLine.size() - 1,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line4Params, cMesh);
newELine.insert(newELine.begin(), sLine[sLine.size() - 2]);
generateElements(eLine, newELine);

newSLine.erase(newSLine.end() - 1);
newELine.erase(newELine.begin());
newNLine.erase(newNLine.begin());
newWLine.erase(newWLine.begin());
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_AOOOP_270() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

auto ref1Vert = getRefVertex(nLine, wLine);
cMesh.addVertex(ref1Vert);

SimpleLine l1(wLine[1], eLine[1]);
SimpleLine l2(sLine[sLine.size() - 2], nLine[nLine.size() - 2]);
auto ref2Vert = findIntersection(l1, l2);
cMesh.addVertex(ref2Vert);

SimpleLine l3(ref1Vert, sLine[1]);
auto ref3Vert = findIntersection(l3, l1);
cMesh.addVertex(ref3Vert);

SimpleLine l4(ref1Vert, eLine[eLine.size() - 2]);
auto ref4Vert = findIntersection(l4, l2);
cMesh.addVertex(ref4Vert);

double sElementSize = getElementSize(
    ref3Vert,
    ref2Vert,
    VertexLineDirection::Horizontal,
    sLine.size() - 2
);
double wElementSize = getElementSize(
    ref3Vert,
    ref1Vert,
    VertexLineDirection::Vertical,
    wLine.size() - 1
);
double nElementSize = getElementSize(
    ref1Vert,
    ref4Vert,
    VertexLineDirection::Horizontal,
    nLine.size() - 1
);
double eElementSize = getElementSize(
    ref2Vert,
    ref4Vert,
    VertexLineDirection::Vertical,
    eLine.size() - 2
);

VertexLineParams Line1Params { 
    ref3Vert,
    ref1Vert,
    wLine.size() - 1,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line1Params, cMesh);
newWLine.insert(newWLine.begin(), sLine[1]);
generateElements(wLine, newWLine);

VertexLineParams Line2Params {
    ref3Vert,
    ref2Vert,
    sLine.size() - 2,
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line2Params, cMesh);
sLine.erase(sLine.begin());
newSLine.push_back(eLine[1]);
generateElements(sLine, newSLine);

VertexLineParams Line3Params {
    ref2Vert,
    ref4Vert,
    eLine.size() - 2,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line3Params, cMesh);
eLine.erase(eLine.begin());
newELine.push_back(nLine[nLine.size() - 2]);
generateElements(eLine, newELine);

VertexLineParams Line4Params {
    ref1Vert,
    ref4Vert,
    nLine.size() - 1,
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line4Params, cMesh);
newNLine.push_back(eLine[eLine.size() - 2]);
generateElements(nLine, newNLine);

newSLine.erase(newSLine.end() - 1);
newELine.erase(newELine.end() - 1);
newNLine.erase(newNLine.end() - 1);
newWLine.erase(newWLine.begin());
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_OOOO_L_0() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

SimpleLine l1(wLine[1], eLine[1]);
SimpleLine l2(sLine[1], nLine[1]);
auto ref1Vert = findIntersection(l1, l2);
cMesh.addVertex(ref1Vert);

double sElementSize = getElementSize(
    eLine[1],
    ref1Vert,
    VertexLineDirection::Horizontal,
    sLine.size() - 1
);
double wElementSize = getElementSize(
    nLine[1],
    ref1Vert,
    VertexLineDirection::Vertical,
    wLine.size() - 1
);

VertexLineParams Line1Params { 
    ref1Vert,
    eLine[1],
    sLine.size() - 1,
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
newSLine.insert(newSLine.begin(), wLine[1]);
generateElements(sLine, newSLine);

VertexLineParams Line2Params {
    ref1Vert,
    nLine[1],
    wLine.size() - 1,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
wLine.erase(wLine.begin());
generateElements(wLine, newWLine);

newSLine.erase(newSLine.begin());
nLine.erase(nLine.begin());
eLine.erase(eLine.begin());
cBoundary.west = newWLine;
cBoundary.south = newSLine;
cBoundary.east = eLine;
cBoundary.north = nLine;
}

void MeshShapesGenerator::gen_OOOO_L_90() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

SimpleLine l1(wLine[1], eLine[1]);
SimpleLine l2(sLine[sLine.size() - 2], nLine[nLine.size() - 2]);
auto ref1Vert = findIntersection(l1, l2);
cMesh.addVertex(ref1Vert);

double sElementSize = getElementSize(
    wLine[1],
    ref1Vert,
    VertexLineDirection::Horizontal,
    sLine.size() - 1
);
double eElementSize = getElementSize(
    nLine[nLine.size() - 2],
    ref1Vert,
    VertexLineDirection::Vertical,
    eLine.size() - 1
);

VertexLineParams Line1Params { 
    wLine[1],
    ref1Vert,
    sLine.size() - 1,
    sElementSize,
    VertexLineDirection::Horizontal
};
auto newSLine = generateVertexLine(Line1Params, cMesh);
newSLine.push_back(eLine[1]);
generateElements(sLine, newSLine);

VertexLineParams Line2Params {
    ref1Vert,
    nLine[nLine.size() - 2],
    eLine.size() - 1,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line2Params, cMesh);
eLine.erase(eLine.begin());
generateElements(eLine, newELine);

nLine.erase(nLine.end() - 1);
newSLine.erase(newSLine.end() - 1);
wLine.erase(wLine.begin());
cBoundary.west = wLine;
cBoundary.south = newSLine;
cBoundary.east = newELine;
cBoundary.north = nLine;
}

void MeshShapesGenerator::gen_OOOO_L_180() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

SimpleLine l1(wLine[wLine.size() - 2], eLine[eLine.size() - 2]);
SimpleLine l2(sLine[sLine.size() - 2], nLine[nLine.size() - 2]);
auto ref1Vert = findIntersection(l1, l2);
cMesh.addVertex(ref1Vert);

double eElementSize = getElementSize(
    ref1Vert,
    sLine[sLine.size() - 2],
    VertexLineDirection::Vertical,
    eLine.size() - 1
);
double nElementSize = getElementSize(
    ref1Vert,
    wLine[wLine.size() - 2],
    VertexLineDirection::Horizontal,
    nLine.size() - 1
);

VertexLineParams Line1Params {
    wLine[wLine.size() - 2],
    ref1Vert,
    nLine.size() - 1,
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line1Params, cMesh);
newNLine.push_back(eLine[eLine.size() - 2]);
generateElements(nLine, newNLine);

VertexLineParams Line3Params {
    sLine[sLine.size() - 2],
    ref1Vert,
    eLine.size() - 1,
    eElementSize,
    VertexLineDirection::Vertical
};
auto newELine = generateVertexLine(Line3Params, cMesh);
eLine.erase(eLine.end() - 1);
generateElements(eLine, newELine);

newNLine.erase(newNLine.end() - 1);
sLine.erase(sLine.end() - 1);
wLine.erase(wLine.end() - 1);
cBoundary.west = wLine;
cBoundary.south = sLine;
cBoundary.east = newELine;
cBoundary.north = newNLine;
}

void MeshShapesGenerator::gen_OOOO_L_270() {

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

SimpleLine l1(wLine[wLine.size() - 2], eLine[eLine.size() - 2]);
SimpleLine l2(sLine[1], nLine[1]);
auto ref1Vert = findIntersection(l1, l2);
cMesh.addVertex(ref1Vert);

double nElementSize = getElementSize(
    eLine[eLine.size() - 2],
    ref1Vert,
    VertexLineDirection::Horizontal,
    nLine.size() - 1
);
double wElementSize = getElementSize(
    ref1Vert,
    sLine[1],
    VertexLineDirection::Vertical,
    wLine.size() - 1
);

VertexLineParams Line1Params {
    ref1Vert,
    eLine[eLine.size() - 2],
    nLine.size() - 1,
    nElementSize,
    VertexLineDirection::Horizontal
};
auto newNLine = generateVertexLine(Line1Params, cMesh);
newNLine.insert(newNLine.begin(), wLine[wLine.size() - 2]);
generateElements(nLine, newNLine);

VertexLineParams Line2Params {
    sLine[1],
    ref1Vert,
    wLine.size() - 1,
    wElementSize,
    VertexLineDirection::Vertical
};
auto newWLine = generateVertexLine(Line2Params, cMesh);
wLine.erase(wLine.end() - 1);
generateElements(wLine, newWLine);

newNLine.erase(newNLine.begin());
sLine.erase(sLine.begin());
eLine.erase(eLine.end() - 1);
cBoundary.west = newWLine;
cBoundary.south = sLine;
cBoundary.east = eLine;
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