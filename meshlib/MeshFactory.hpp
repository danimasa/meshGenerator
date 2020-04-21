#pragma once

#include "Vertex.hpp"
#include "Point.hpp"
#include "Line.hpp"
#include "QuadArea.hpp"
#include "Area.hpp"
#include "Mesh.hpp"

namespace meshlib {

using namespace geomlib;
using namespace std;

class MeshFactory {
public:
    static MeshFactory* getDefaultInstance();

    void setElementSize(double elementSize);

    Vertex* genVertex(Point *point) const;
    Vertex* genVertex(double x, double y, double z) const;

    vector<Vertex*> subdivideLine(Line *line) const;
    vector<Vertex*> subdivideLine(Line *line, int qtdElements) const;

    Mesh* genSurfaceMesh(QuadArea *area) const;

    QuadArea* subdivideArea(Area *area) const;

    MeshFactory(MeshFactory const&) = delete;
    void operator=(MeshFactory const&) = delete;
private:
    MeshFactory() {}

    vector<Vertex*> generateLineVertices(
        Line* line,
        int qtdElements,
        double lastXPos = 0.0,
        double pPosition = 1.0,
        bool direct = true) const;

    double elementSize = 0.0;
};

}