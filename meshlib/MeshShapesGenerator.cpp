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

if(cBoundary.south.size() + 1 > cBoundary.north.size()
    || cBoundary.west.size() + 1 > cBoundary.east.size()) {
    throw std::invalid_argument("POOO_0_TYPE: north and east boundary should have at least 1 more elements.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

vector<Vertex*> newVLine;
double sxLength = sLine[sLine.size() - 1]->x - sLine[0]->x;
double syLenght = wLine[wLine.size() - 1]->y - wLine[0]->y;
double vElementSize = syLenght / wLine.size();
double hElementSize = sxLength / sLine.size();
double initXPos = sLine[0]->x + hElementSize;
double finalXPos = nLine[1]->x;
double initYPos = wLine[0]->y + vElementSize;
double finalYPos = eLine[1]->y;

Vertex* refVert = new Vertex(initXPos, initYPos, 0);
cMesh.vertices.push_back(refVert);
newVLine.push_back(refVert);

double vElementPos = initYPos;
int n = wLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += vElementSize;
    double cXPos = initXPos + i * (finalXPos - initXPos) / n;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLine.push_back(v);
}
newVLine.push_back(nLine[1]);

vector<Vertex*> newHLine;
newHLine.push_back(refVert);

vElementPos = initXPos;
n = sLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += hElementSize;
    double cYPos = initYPos + i * (finalYPos - initYPos) / n;
    auto v = new Vertex(vElementPos, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLine.push_back(v);
}
newHLine.push_back(eLine[1]);

generateElements(wLine, newVLine, cMesh);
generateElements(sLine, newHLine, cMesh);

cBoundary.west = newVLine;
cBoundary.south = newHLine;
cBoundary.east.erase(cBoundary.east.begin());
cBoundary.north.erase(cBoundary.north.begin());

}

void MeshShapesGenerator::gen_POOO_90() {

if(cBoundary.south.size() + 1 > cBoundary.north.size()
    || cBoundary.east.size() + 1 > cBoundary.west.size()) {
    throw std::invalid_argument("POOO_90_TYPE: north and west boundary should have at least 1 more elements.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

int lastIndN = nLine.size() - 2;
vector<Vertex*> newVLine;
double sxLength = sLine[sLine.size() - 1]->x - sLine[0]->x;
double syLenght = eLine[eLine.size() - 1]->y - eLine[0]->y;
double vElementSize = syLenght / eLine.size();
double hElementSize = sxLength / sLine.size();
double initXPos = sLine[0]->x + hElementSize * (sLine.size() - 1);
double finalXPos = nLine[lastIndN]->x;
double initYPos = wLine[1]->y;
double finalYPos = eLine[0]->y + vElementSize;

Vertex* refVert = new Vertex(initXPos, finalYPos, 0);
cMesh.vertices.push_back(refVert);
newVLine.push_back(refVert);

double vElementPos = finalYPos;
int n = eLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += vElementSize;
    double cXPos = initXPos + i * (finalXPos - initXPos) / n;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLine.push_back(v);
}
newVLine.push_back(nLine[lastIndN]);

generateElements(eLine, newVLine, cMesh);

vector<Vertex*> newHLine;
newHLine.push_back(wLine[1]);

vElementPos = wLine[1]->x;
n = sLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += hElementSize;
    double cYPos = initYPos + i * (finalYPos - initYPos) / n;
    auto v = new Vertex(vElementPos, cYPos, 0);
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

if(cBoundary.north.size() + 1 > cBoundary.south.size()
    || cBoundary.east.size() + 1 > cBoundary.west.size()) {
    throw std::invalid_argument("POOO_180_TYPE: south and west boundary should have at least 1 more elements.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

int lastIndN = nLine.size() - 2;
int lastIndE = eLine.size() - 2;
int lastIndW = wLine.size() - 2;
int lastIndS = sLine.size() - 2;

double sxLength = nLine[nLine.size() - 1]->x - nLine[0]->x;
double syLenght = eLine[eLine.size() - 1]->y - eLine[0]->y;
double vElementSize = syLenght / eLine.size();
double hElementSize = sxLength / nLine.size();
double initXPos = sLine[lastIndS]->x;
double finalXPos = nLine[0]->x + hElementSize * (nLine.size() - 1);
double initYPos = wLine[lastIndW]->y;
double finalYPos = eLine[0]->y + vElementSize * (eLine.size() - 1);

vector<Vertex*> newVLine;
Vertex* refVert = new Vertex(finalXPos, finalYPos, 0);
cMesh.vertices.push_back(refVert);

double vElementPos = sLine[lastIndS]->y;
int n = eLine.size() - 1;

newVLine.push_back(sLine[lastIndS]);
for(int i = 1; i < n; i++) {
    vElementPos += vElementSize;
    double cXPos = initXPos + i * (finalXPos - initXPos) / n;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLine.push_back(v);
}
newVLine.push_back(refVert);

generateElements(eLine, newVLine, cMesh);

vector<Vertex*> newHLine;
newHLine.push_back(wLine[lastIndW]);

vElementPos = wLine[lastIndW]->x;
n = nLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += vElementSize;
    double cYPos = initYPos + i * (finalYPos - initYPos) / n;
    auto v = new Vertex(vElementPos, cYPos, 0);
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

if(cBoundary.north.size() + 1 > cBoundary.south.size()
    || cBoundary.west.size() + 1 > cBoundary.east.size()) {
    throw std::invalid_argument("POOO_270_TYPE: south and east boundary should have at least 1 more elements.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

int lastIndE = eLine.size() - 2;
double sxLength = nLine[nLine.size() - 1]->x - nLine[0]->x;
double syLenght = wLine[wLine.size() - 1]->y - wLine[0]->y;
double vElementSize = syLenght / wLine.size();
double hElementSize = sxLength / nLine.size();
double initXPos = sLine[1]->x;
double finalXPos = nLine[0]->x + hElementSize;
double initYPos = wLine[0]->y + vElementSize * (wLine.size() - 1);
double finalYPos = eLine[lastIndE]->y;

vector<Vertex*> newVLine;

Vertex* refVert = new Vertex(finalXPos, initYPos, 0);
cMesh.vertices.push_back(refVert);

double vElementPos = sLine[1]->y;
int n = wLine.size() - 1;

newVLine.push_back(sLine[1]);
for(int i = 1; i < n; i++) {
    vElementPos += vElementSize;
    double cXPos = initXPos + i * (finalXPos - initXPos) / n;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLine.push_back(v);
}
newVLine.push_back(refVert);

generateElements(wLine, newVLine, cMesh);

vector<Vertex*> newHLine;
newHLine.push_back(refVert);

vElementPos = refVert->x;
n = nLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += vElementSize;
    double cYPos = initYPos + i * (finalYPos - initYPos) / n;
    auto v = new Vertex(vElementPos, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLine.push_back(v);
}
newHLine.push_back(eLine[lastIndE]);

generateElements(nLine, newHLine, cMesh);

cBoundary.east.erase(cBoundary.east.end() - 1);
cBoundary.south.erase(cBoundary.south.begin());
cBoundary.west = newVLine;
cBoundary.north = newHLine;

}

void MeshShapesGenerator::gen_PPOO_0() {

if(cBoundary.south.size() + 2 > cBoundary.north.size()
    || cBoundary.east.size() != cBoundary.west.size()) {
    throw std::invalid_argument("PPOO_0_TYPE: north should have at least 2 more elements than south, equal east and west.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

double sxLength = sLine[sLine.size() - 1]->x - sLine[0]->x;
double swyLenght = wLine[wLine.size() - 1]->y - wLine[0]->y;
double seyLenght = eLine[eLine.size() - 1]->y - eLine[0]->y;
double vwElementSize = swyLenght / wLine.size();
double veElementSize = seyLenght / eLine.size();
double hElementSize = sxLength / (sLine.size() + 1);
double initXPos = sLine[0]->x + hElementSize;
double finalXPos = nLine[1]->x;
double initYPos = wLine[0]->y + vwElementSize;
double finalYPos = eLine[0]->y + veElementSize;

vector<Vertex*> newVLineWest;

Vertex* refVertWest = new Vertex(initXPos, initYPos, 0);
cMesh.vertices.push_back(refVertWest);

Vertex* lastVertWest = nLine[1];
double vElementPos = initYPos;
int n = wLine.size() - 1;

newVLineWest.push_back(refVertWest);
for(int i = 1; i < n; i++) {
    vElementPos += vwElementSize;
    double cXPos = initXPos + i * (finalXPos - initXPos) / n;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLineWest.push_back(v);
}
newVLineWest.push_back(nLine[1]);

generateElements(wLine, newVLineWest, cMesh);

int lastXInd = nLine.size() - 2;
initXPos = sLine[0]->x + hElementSize * sLine.size();
finalXPos = nLine[lastXInd]->x;
Vertex* refVertEast = new Vertex(initXPos, finalYPos, 0);
cMesh.vertices.push_back(refVertEast);

vector<Vertex*> newVLineEast;
newVLineEast.push_back(refVertEast);
vElementPos = initYPos;
n = eLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += veElementSize;
    double cXPos = initXPos + i * (finalXPos - initXPos) / n;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLineEast.push_back(v);
}
newVLineEast.push_back(nLine[lastXInd]);

generateElements(eLine, newVLineEast, cMesh);

vector<Vertex*> newHLine;
newHLine.push_back(refVertWest);

vElementPos = refVertWest->x;
n = sLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += hElementSize;
    double cYPos = initYPos + i * (finalYPos - initYPos) / n;
    auto v = new Vertex(vElementPos, cYPos, 0);
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

if(cBoundary.east.size() + 2 > cBoundary.west.size()
    || cBoundary.south.size() != cBoundary.north.size()) {
    throw std::invalid_argument("PPOO_90_TYPE: west should have at least 2 more elements than east, equal north and south.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

double ssxLength = sLine[sLine.size() - 1]->x - sLine[0]->x;
double snxLenght = nLine[nLine.size() - 1]->x - nLine[0]->x;
double syLenght = eLine[eLine.size() - 1]->y - eLine[0]->y;
double vElementSize = syLenght / (eLine.size() + 1);
double hsElementSize = ssxLength / sLine.size();
double hnElementSize = snxLenght / nLine.size();
double initXPos = sLine[0]->x + hsElementSize * (sLine.size() - 1);
double finalXPos = nLine[0]->x + hnElementSize * (nLine.size() - 1);
double initYPos = wLine[1]->y;
double finalYPos = eLine[0]->y + vElementSize;

vector<Vertex*> newHLineSouth;
Vertex* refVertSouth = new Vertex(initXPos, finalYPos, 0);
cMesh.vertices.push_back(refVertSouth);

Vertex* lastVertSouth = wLine[1];
double vElementPos = lastVertSouth->x;
int n = sLine.size() - 1;

newHLineSouth.push_back(wLine[1]);
for(int i = 1; i < n; i++) {
    vElementPos += hsElementSize;
    double cYPos = initYPos + i * (finalYPos - initYPos) / n;
    auto v = new Vertex(vElementPos, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLineSouth.push_back(v);
}
newHLineSouth.push_back(refVertSouth);

generateElements(sLine, newHLineSouth, cMesh);

int lastYInd = wLine.size() - 2;
initYPos = wLine[lastYInd]->y;
finalYPos = eLine[0]->y + vElementSize * eLine.size();
Vertex* refVertNorth = new Vertex(finalXPos, finalYPos, 0);
cMesh.vertices.push_back(refVertNorth);

vector<Vertex*> newHLineNorth;
newHLineNorth.push_back(wLine[lastYInd]);
vElementPos = wLine[lastYInd]->x;
n = nLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += hnElementSize;
    double cYPos = initYPos + i * (finalYPos - initYPos) / n;
    auto v = new Vertex(vElementPos, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLineNorth.push_back(v);
}
newHLineNorth.push_back(refVertNorth);

generateElements(nLine, newHLineNorth, cMesh);

vector<Vertex*> newVLine;
newVLine.push_back(refVertSouth);

vElementPos = refVertSouth->y;
n = eLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += vElementSize;
    double cXPos = initXPos + i * (finalXPos - initXPos) / n;
    auto v = new Vertex(cXPos, vElementPos, 0);
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

if(cBoundary.north.size() + 2 > cBoundary.south.size()
    || cBoundary.east.size() != cBoundary.west.size()) {
    throw std::invalid_argument("PPOO_180_TYPE: south should have at least 2 more elements than north, equal east and west.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

double sxLength = nLine[nLine.size() - 1]->x - nLine[0]->x;
double swyLenght = wLine[wLine.size() - 1]->y - wLine[0]->y;
double seyLenght = eLine[eLine.size() - 1]->y - eLine[0]->y;
double vwElementSize = swyLenght / wLine.size();
double veElementSize = seyLenght / eLine.size();
double hElementSize = sxLength / (nLine.size() + 1);
double initXPos = sLine[1]->x;
double finalXPos = nLine[0]->x + hElementSize;
double initYPos = wLine[0]->y + vwElementSize * (wLine.size() - 1);
double finalYPos = eLine[0]->y + veElementSize * (eLine.size() - 1);

vector<Vertex*> newVLineWest;

Vertex* refVertWest = new Vertex(finalXPos, initYPos, 0);
cMesh.vertices.push_back(refVertWest);

Vertex* firstVertWest = sLine[1];
double vElementPos = firstVertWest->y;
int n = wLine.size() - 1;

newVLineWest.push_back(sLine[1]);
for(int i = 1; i < n; i++) {
    vElementPos += vwElementSize;
    double cXPos = initXPos + i * (finalXPos - initXPos) / n;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLineWest.push_back(v);
}
newVLineWest.push_back(refVertWest);

generateElements(wLine, newVLineWest, cMesh);

int lastXInd = sLine.size() - 2;
initXPos = sLine[lastXInd]->x;
finalXPos = nLine[0]->x + hElementSize * nLine.size();
Vertex* refVertEast = new Vertex(finalXPos, finalYPos, 0);
cMesh.vertices.push_back(refVertEast);

vector<Vertex*> newVLineEast;
newVLineEast.push_back(sLine[lastXInd]);
vElementPos = sLine[lastXInd]->y;
n = eLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += veElementSize;
    double cXPos = initXPos + i * (finalXPos - initXPos) / n;
    auto v = new Vertex(cXPos, vElementPos, 0);
    cMesh.vertices.push_back(v);
    newVLineEast.push_back(v);
}
newVLineEast.push_back(refVertEast);

generateElements(eLine, newVLineEast, cMesh);

vector<Vertex*> newHLine;
newHLine.push_back(refVertWest);

vElementPos = refVertWest->x;
n = nLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += hElementSize;
    double cYPos = initYPos + i * (finalYPos - initYPos) / n;
    auto v = new Vertex(vElementPos, cYPos, 0);
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

if(cBoundary.west.size() + 2 > cBoundary.east.size()
    || cBoundary.south.size() != cBoundary.north.size()) {
    throw std::invalid_argument("PPOO_270_TYPE: east should have at least 2 more elements than west, equal north and south.");
}

auto sLine = cBoundary.south;
auto wLine = cBoundary.west;
auto eLine = cBoundary.east;
auto nLine = cBoundary.north;

double ssxLength = sLine[sLine.size() - 1]->x - sLine[0]->x;
double snxLenght = nLine[nLine.size() - 1]->x - nLine[0]->x;
double syLenght = wLine[wLine.size() - 1]->y - wLine[0]->y;
double vElementSize = syLenght / (wLine.size() + 1);
double hsElementSize = ssxLength / sLine.size();
double hnElementSize = snxLenght / nLine.size();
double initXPos = sLine[0]->x + hsElementSize;
double finalXPos = nLine[0]->x + hnElementSize;
double initYPos = wLine[0]->y + vElementSize;
double finalYPos = eLine[1]->y;

vector<Vertex*> newHLineSouth;
Vertex* refVertSouth = new Vertex(initXPos, initYPos, 0);
cMesh.vertices.push_back(refVertSouth);

double vElementPos = initXPos;
int n = sLine.size() - 1;

newHLineSouth.push_back(refVertSouth);
for(int i = 1; i < n; i++) {
    vElementPos += hsElementSize;
    double cYPos = initYPos + i * (finalYPos - initYPos) / n;
    auto v = new Vertex(vElementPos, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLineSouth.push_back(v);
}
newHLineSouth.push_back(eLine[1]);

generateElements(sLine, newHLineSouth, cMesh);

int lastYInd = eLine.size() - 2;
initYPos = wLine[0]->y + vElementSize * wLine.size();
finalYPos = eLine[lastYInd]->y;
Vertex* refVertNorth = new Vertex(finalXPos, initYPos, 0);
cMesh.vertices.push_back(refVertNorth);

vector<Vertex*> newHLineNorth;
newHLineNorth.push_back(refVertNorth);
vElementPos = finalXPos;
n = nLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += hnElementSize;
    double cYPos = initYPos + i * (finalYPos - initYPos) / n;
    auto v = new Vertex(vElementPos, cYPos, 0);
    cMesh.vertices.push_back(v);
    newHLineNorth.push_back(v);
}
newHLineNorth.push_back(eLine[lastYInd]);

generateElements(nLine, newHLineNorth, cMesh);

vector<Vertex*> newVLine;
newVLine.push_back(refVertSouth);

vElementPos = refVertSouth->y;
n = wLine.size() - 1;

for(int i = 1; i < n; i++) {
    vElementPos += vElementSize;
    double cXPos = initXPos + i * (finalXPos - initXPos) / n;
    auto v = new Vertex(cXPos, vElementPos, 0);
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