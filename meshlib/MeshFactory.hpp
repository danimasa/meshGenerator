#pragma once

#include <stdexcept>

#include "Area.hpp"
#include "GeometryList.hpp"
#include "Line.hpp"
#include "Mesh.hpp"
#include "meshlib.hpp"
#include "Point.hpp"
#include "QuadArea.hpp"
#include "Vertex.hpp"
#include "AppParams.hpp"

namespace meshlib {

using namespace geomlib;
using namespace std;

class MeshFactory {
public:
    static MeshFactory* getDefaultInstance();
    static void init(AppParams& params);

    Vertex* genVertex(Point *point) const;
    Vertex* genVertex(double x, double y, double z) const;

    vector<Vertex*> generateLineElements(Line *line) const;

    Mesh* genSurfaceMesh(QuadArea *area) const;

    QuadArea* subdivideArea(Area *area) const;
    void evenElementsInArea(GeometryList* geomList);

    MeshFactory(MeshFactory const&) = delete;
    void operator=(MeshFactory const&) = delete;
private:
    AppParams _appParams;

    static MeshFactory* getInstanceImpl(AppParams& params);
    MeshFactory(AppParams& params)
        : _appParams(params)
    {
        if(params.globalElementSize <= 0)
            throw std::runtime_error{ "globalElementSize must be greater than 0"};
    }

    vector<Vertex*> m_generateLineElements(
        Line* line,
        double pPosition = 1.0,
        bool direct = true) const;

    void m_evenElementsByNumberOfContacts(vector<Line*> &okLines, int nContacts, GeometryList* geomList);
};

}