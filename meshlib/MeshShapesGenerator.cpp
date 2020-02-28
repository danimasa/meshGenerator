#include "MeshShapesGenerator.hpp"

#include <stdexcept>

#include "meshlib.hpp"

namespace meshlib {

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

Mesh MeshShapesGenerator::genMesh(std::vector<MeshShapes::RelativeShapes> shapeList
    , const AreaMesh &area) {

    cBoundary = generateMesheableBoundary(area);

    addVerticesToMesh(cBoundary.south, cMesh);
    addVerticesToMesh(cBoundary.east, cMesh);
    addVerticesToMesh(cBoundary.north, cMesh);
    addVerticesToMesh(cBoundary.west, cMesh);

    for(auto shape : shapeList) {
        switch (shape)
        {
        case MeshShapes::RelativeShapes::POOO_0:
            gen_POOO_0();
            break;
        case MeshShapes::RelativeShapes::POOO_90:
            gen_POOO_90();
            break;
        case MeshShapes::RelativeShapes::POOO_180:
            gen_POOO_180();
            break;
        case MeshShapes::RelativeShapes::POOO_270:
            gen_POOO_270();
            break;
        case MeshShapes::RelativeShapes::PPOO_0:
            gen_PPOO_0();
            break;
        case MeshShapes::RelativeShapes::PPOO_90:
            gen_PPOO_90();
            break;
        case MeshShapes::RelativeShapes::PPOO_180:
            gen_PPOO_180();
            break;
        case MeshShapes::RelativeShapes::PPOO_270:
            gen_PPOO_270();
            break;
        case MeshShapes::RelativeShapes::OOOO:
            gen_OOOO();
            break;
        default:
            break;
        }
    }

    return cMesh;
}

void MeshShapesGenerator::gen_POOO_0() {

if(cBoundary.north.size() + 1 != cBoundary.south.size()
    || cBoundary.east.size() + 1 != cBoundary.west.size()) {
    throw new std::invalid_argument("POOO_0_TYPE: north and east boundary should have 1 more elements.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

vector<Vertex*> newVLine;
double cYPos = eLine[1]->y;
double cXPos = nLine[1]->x;

Vertex* refVert = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVert);
newVLine.push_back(refVert);

for(int i = 2; i < eLine.size() - 1; i++) {
    auto v = new Vertex(cXPos, eLine[i]->y, 0);
    cMesh.vertices.push_back(v);
    newVLine.push_back(v);
}
newVLine.push_back(nLine[1]);

generateElements(wLine, newVLine, cMesh);

vector<Vertex*> newHLine;
newHLine.push_back(refVert);

cYPos = refVert->y;
cXPos = refVert->x;

for(int i = 2; i < nLine.size() - 1; i++) {
    auto v = new Vertex(nLine[i]->x, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLine.push_back(v);
}
newHLine.push_back(eLine[1]);

generateElements(sLine, newHLine, cMesh);

cBoundary.west = newVLine;
cBoundary.south = newHLine;
cBoundary.east.erase(cBoundary.east.begin());
cBoundary.north.erase(cBoundary.north.begin());

}

void MeshShapesGenerator::gen_POOO_90() {

if(cBoundary.north.size() + 1 != cBoundary.south.size()
    || cBoundary.west.size() + 1 != cBoundary.east.size()) {
    throw new std::invalid_argument("POOO_90_TYPE: north and west boundary should have 1 more elements.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

int lastXInd = nLine.size() - 2;
vector<Vertex*> newVLine;
double cYPos = wLine[1]->y;
double cXPos = nLine[lastXInd]->x;

Vertex* refVert = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVert);
newVLine.push_back(refVert);

for(int i = 2; i < wLine.size() - 1; i++) {
    auto v = new Vertex(cXPos, wLine[i]->y, 0);
    cMesh.vertices.push_back(v);
    newVLine.push_back(v);
}
newVLine.push_back(nLine[lastXInd]);

generateElements(eLine, newVLine, cMesh);

vector<Vertex*> newHLine;
newHLine.push_back(wLine[1]);

cYPos = refVert->y;
cXPos = refVert->x;

for(int i = 1; i < nLine.size() - 2; i++) {
    auto v = new Vertex(nLine[i]->x, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLine.push_back(v);
}
newHLine.push_back(refVert);

generateElements(sLine, newHLine, cMesh);

cBoundary.west.erase(cBoundary.west.begin());
cBoundary.south = newHLine;
cBoundary.east = newVLine;
cBoundary.north.erase(cBoundary.north.end() - 1);

}

void MeshShapesGenerator::gen_POOO_180() {

if(cBoundary.south.size() + 1 != cBoundary.north.size()
    || cBoundary.west.size() + 1 != cBoundary.east.size()) {
    throw new std::invalid_argument("POOO_180_TYPE: south and west boundary should have 1 more elements.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

int lastXInd = sLine.size() - 2;
int lastYInd = wLine.size() - 2;
vector<Vertex*> newVLine;
double cYPos = wLine[lastYInd]->y;
double cXPos = sLine[lastXInd]->x;

Vertex* refVert = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVert);

newVLine.push_back(sLine[lastXInd]);
for(int i = 1; i < wLine.size() - 2; i++) {
    auto v = new Vertex(cXPos, wLine[i]->y, 0);
    cMesh.vertices.push_back(v);
    newVLine.push_back(v);
}
newVLine.push_back(refVert);

generateElements(eLine, newVLine, cMesh);

vector<Vertex*> newHLine;
newHLine.push_back(wLine[lastYInd]);

cYPos = refVert->y;
cXPos = refVert->x;

for(int i = 1; i < sLine.size() - 2; i++) {
    auto v = new Vertex(sLine[i]->x, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLine.push_back(v);
}
newHLine.push_back(refVert);

generateElements(nLine, newHLine, cMesh);

cBoundary.west.erase(cBoundary.west.end() - 1);
cBoundary.south.erase(cBoundary.south.end() - 1);
cBoundary.east = newVLine;
cBoundary.north = newHLine;

}

void MeshShapesGenerator::gen_POOO_270() {

if(cBoundary.south.size() + 1 != cBoundary.north.size()
    || cBoundary.east.size() + 1 != cBoundary.west.size()) {
    throw new std::invalid_argument("POOO_270_TYPE: south and east boundary should have 1 more elements.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

int lastYInd = eLine.size() - 2;
vector<Vertex*> newVLine;
double cYPos = eLine[lastYInd]->y;
double cXPos = sLine[1]->x;

Vertex* refVert = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVert);

newVLine.push_back(sLine[1]);
for(int i = 1; i < eLine.size() - 2; i++) {
    auto v = new Vertex(cXPos, eLine[i]->y, 0);
    cMesh.vertices.push_back(v);
    newVLine.push_back(v);
}
newVLine.push_back(refVert);

generateElements(wLine, newVLine, cMesh);

vector<Vertex*> newHLine;
newHLine.push_back(refVert);

cYPos = refVert->y;
cXPos = refVert->x;

for(int i = 2; i < sLine.size() - 1; i++) {
    auto v = new Vertex(sLine[i]->x, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLine.push_back(v);
}
newHLine.push_back(eLine[lastYInd]);

generateElements(nLine, newHLine, cMesh);

cBoundary.east.erase(cBoundary.east.end() - 1);
cBoundary.south.erase(cBoundary.south.begin());
cBoundary.west = newVLine;
cBoundary.north = newHLine;

}

void MeshShapesGenerator::gen_PPOO_0() {

if(cBoundary.south.size() + 2 != cBoundary.north.size()
    || cBoundary.east.size() != cBoundary.west.size()) {
    throw new std::invalid_argument("PPOO_0_TYPE: south 2 more elements than north, equal east and west.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

vector<Vertex*> newVLineWest;
double cYPos = wLine[1]->y;
double cXPos = nLine[1]->x;

Vertex* refVertWest = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVertWest);

Vertex* lastVertWest = nLine[1];
double vLineSize = lastVertWest->y - refVertWest->y;
double vElementSize = vLineSize / (eLine.size() - 1);
double vElementPos = cYPos;

newVLineWest.push_back(refVertWest);
for(int i = 1; i < wLine.size() - 1; i++) {
    vElementPos += vElementSize;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLineWest.push_back(v);
}
newVLineWest.push_back(nLine[1]);

generateElements(wLine, newVLineWest, cMesh);

int lastXInd = nLine.size() - 2;
cXPos = nLine[lastXInd]->x;
Vertex* refVertEast = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVertEast);

vector<Vertex*> newVLineEast;
newVLineEast.push_back(refVertEast);
vElementPos = cYPos;
for(int i = 1; i < wLine.size() - 1; i++) {
    vElementPos += vElementSize;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLineEast.push_back(v);
}
newVLineEast.push_back(nLine[lastXInd]);

generateElements(eLine, newVLineEast, cMesh);

vector<Vertex*> newHLine;
newHLine.push_back(refVertWest);

cYPos = refVertWest->y;

for(int i = 2; i < nLine.size() - 2; i++) {
    auto v = new Vertex(nLine[i]->x, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLine.push_back(v);
}
newHLine.push_back(refVertEast);

generateElements(sLine, newHLine, cMesh);

cBoundary.east = newVLineEast;
cBoundary.south = newHLine;
cBoundary.west = newVLineWest;
cBoundary.north.erase(cBoundary.north.begin());
cBoundary.north.erase(cBoundary.north.end() - 1);
}

void MeshShapesGenerator::gen_PPOO_90() {

if(cBoundary.east.size() + 2 != cBoundary.west.size()
    || cBoundary.south.size() != cBoundary.north.size()) {
    throw new std::invalid_argument("PPOO_90_TYPE: west 2 more elements than east, equal north and south.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

int lastXInd = nLine.size() - 2;

vector<Vertex*> newHLineSouth;
double cYPos = wLine[1]->y;
double cXPos = nLine[lastXInd]->x;

Vertex* refVertSouth = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVertSouth);

Vertex* lastVertSouth = wLine[1];
double vLineSize = refVertSouth->x - lastVertSouth->x;
double vElementSize = vLineSize / (sLine.size() - 1);
double vElementPos = 0.0;

newHLineSouth.push_back(wLine[1]);
for(int i = 1; i < sLine.size() - 1; i++) {
    vElementPos += vElementSize;
    auto v = new Vertex(vElementPos, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLineSouth.push_back(v);
}
newHLineSouth.push_back(refVertSouth);

generateElements(sLine, newHLineSouth, cMesh);

int lastYInd = wLine.size() - 2;
cYPos = wLine[lastYInd]->y;
Vertex* refVertNorth = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVertNorth);

vector<Vertex*> newHLineNorth;
newHLineNorth.push_back(wLine[lastYInd]);
vElementPos = 0;
for(int i = 1; i < nLine.size() - 1; i++) {
    vElementPos += vElementSize;
    auto v = new Vertex(vElementPos, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLineNorth.push_back(v);
}
newHLineNorth.push_back(refVertNorth);

generateElements(nLine, newHLineNorth, cMesh);

vector<Vertex*> newVLine;
newVLine.push_back(refVertSouth);

cXPos = refVertSouth->x;

for(int i = 2; i < wLine.size() - 2; i++) {
    auto v = new Vertex(cXPos, wLine[i]->y, 0);
    cMesh.vertices.push_back(v);
    newVLine.push_back(v);
}
newVLine.push_back(refVertNorth);

generateElements(eLine, newVLine, cMesh);

cBoundary.east = newVLine;
cBoundary.south = newHLineSouth;
cBoundary.west.erase(cBoundary.west.begin());
cBoundary.west.erase(cBoundary.west.end() - 1);
cBoundary.north = newHLineNorth;
}

void MeshShapesGenerator::gen_PPOO_180() {

if(cBoundary.north.size() + 2 != cBoundary.south.size()
    || cBoundary.east.size() != cBoundary.west.size()) {
    throw new std::invalid_argument("PPOO_180_TYPE: north 2 more elements than south, equal east and west.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

int lastYInd = wLine.size() - 2;
vector<Vertex*> newVLineWest;
double cYPos = wLine[lastYInd]->y;
double cXPos = sLine[1]->x;

Vertex* refVertWest = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVertWest);

Vertex* firstVertWest = sLine[1];
double vLineSize = refVertWest->y - firstVertWest->y ;
double vElementSize = vLineSize / (wLine.size() - 1);
double vElementPos = 0.0;

newVLineWest.push_back(sLine[1]);
for(int i = 1; i < eLine.size() - 1; i++) {
    vElementPos += vElementSize;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLineWest.push_back(v);
}
newVLineWest.push_back(refVertWest);

generateElements(wLine, newVLineWest, cMesh);

int lastXInd = sLine.size() - 2;
cXPos = nLine[lastXInd]->x;
Vertex* refVertEast = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVertEast);

vector<Vertex*> newVLineEast;
newVLineEast.push_back(sLine[lastXInd]);
vElementPos = 0.0;
for(int i = 1; i < eLine.size() - 1; i++) {
    vElementPos += vElementSize;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLineEast.push_back(v);
}
newVLineEast.push_back(refVertEast);

generateElements(eLine, newVLineEast, cMesh);

vector<Vertex*> newHLine;
newHLine.push_back(refVertWest);

cYPos = refVertWest->y;

for(int i = 2; i < sLine.size() - 2; i++) {
    auto v = new Vertex(sLine[i]->x, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLine.push_back(v);
}
newHLine.push_back(refVertEast);

generateElements(nLine, newHLine, cMesh);

cBoundary.east = newVLineEast;
cBoundary.north = newHLine;
cBoundary.west = newVLineWest;
cBoundary.south.erase(cBoundary.south.begin());
cBoundary.south.erase(cBoundary.south.end() - 1);
}

void MeshShapesGenerator::gen_PPOO_270() {

if(cBoundary.west.size() + 2 != cBoundary.east.size()
    || cBoundary.south.size() != cBoundary.north.size()) {
    throw new std::invalid_argument("PPOO_270_TYPE: east 2 more elements than west, equal north and south.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

vector<Vertex*> newHLineSouth;
double cYPos = eLine[1]->y;
double cXPos = nLine[1]->x;

Vertex* refVertSouth = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVertSouth);

Vertex* lastVertSouth = eLine[1];
double vLineSize = lastVertSouth-> x - refVertSouth->x;
double vElementSize = vLineSize / (sLine.size() - 1);
double vElementPos = cXPos;

newHLineSouth.push_back(refVertSouth);
for(int i = 1; i < sLine.size() - 1; i++) {
    vElementPos += vElementSize;
    auto v = new Vertex(vElementPos, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLineSouth.push_back(v);
}
newHLineSouth.push_back(eLine[1]);

generateElements(sLine, newHLineSouth, cMesh);

int lastYInd = eLine.size() - 2;
cYPos = eLine[lastYInd]->y;
Vertex* refVertNorth = new Vertex(cXPos, cYPos, 0);
cMesh.vertices.push_back(refVertNorth);

vector<Vertex*> newHLineNorth;
newHLineNorth.push_back(refVertNorth);
vElementPos = cXPos;
for(int i = 1; i < nLine.size() - 1; i++) {
    vElementPos += vElementSize;
    auto v = new Vertex(vElementPos, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLineNorth.push_back(v);
}
newHLineNorth.push_back(eLine[lastYInd]);

generateElements(nLine, newHLineNorth, cMesh);

vector<Vertex*> newVLine;
newVLine.push_back(refVertSouth);

cXPos = refVertSouth->x;

for(int i = 2; i < eLine.size() - 2; i++) {
    auto v = new Vertex(cXPos, eLine[i]->y, 0);
    cMesh.vertices.push_back(v);
    newVLine.push_back(v);
}
newVLine.push_back(refVertNorth);

generateElements(wLine, newVLine, cMesh);

cBoundary.west = newVLine;
cBoundary.south = newHLineSouth;
cBoundary.east.erase(cBoundary.east.begin());
cBoundary.east.erase(cBoundary.east.end() - 1);
cBoundary.north = newHLineNorth;
}

void MeshShapesGenerator::gen_POPO_0() {

}

void MeshShapesGenerator::gen_POPO_90() {

}

void MeshShapesGenerator::gen_POPP_0() {

}

void MeshShapesGenerator::gen_POPP_90() {

}

void MeshShapesGenerator::gen_POPP_180() {

}

void MeshShapesGenerator::gen_POPP_270() {

}

void MeshShapesGenerator::gen_PPPP() {

}

void MeshShapesGenerator::gen_OOOO() {

if(cBoundary.south.size() != cBoundary.north.size()
    || cBoundary.east.size() != cBoundary.west.size()) {
    throw new std::invalid_argument("OOOO_TYPE: only equal sided elements quantity permitted.");
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
        auto v = new Vertex(sLine[xInd]->x, wLine[yInd]->y, 0);
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