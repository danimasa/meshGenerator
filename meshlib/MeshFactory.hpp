#pragma once

#include "Area.hpp"
#include "GeometryList.hpp"
#include "Line.hpp"
#include "Mesh.hpp"
#include "Point.hpp"
#include "QuadArea.hpp"
#include "Vertex.hpp"

namespace meshlib {

using namespace geomlib;
using namespace std;

class MeshFactory {
public:
    static MeshFactory* getDefaultInstance();

    void setElementSize(double elementSize);

    Vertex* genVertex(Point *point) const;
    Vertex* genVertex(double x, double y, double z) const;

    void fillLineElementsQty(Line *line) const;
    vector<Vertex*> generateLineElements(Line *line) const;

    Mesh* genSurfaceMesh(QuadArea *area) const;

    QuadArea* subdivideArea(Area *area) const;
    void evenElementsInArea(GeometryList* geomList);

    MeshFactory(MeshFactory const&) = delete;
    void operator=(MeshFactory const&) = delete;
private:
    MeshFactory() {}

    vector<Vertex*> m_generateLineElements(
        Line* line,
        double pPosition = 1.0,
        bool direct = true) const;

    void m_fillLineElementsQty(Line* line) const;
    void m_evenElementsByNumberOfContacts(vector<Line*> &okLines, int nContacts, GeometryList* geomList);

    double elementSize = 0.0;
};

}