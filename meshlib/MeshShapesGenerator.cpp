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

}

void MeshShapesGenerator::gen_PPOO_0() {

}

void MeshShapesGenerator::gen_PPOO_90() {

}

void MeshShapesGenerator::gen_PPOO_180() {

}

void MeshShapesGenerator::gen_PPOO_270() {

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