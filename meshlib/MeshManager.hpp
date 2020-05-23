#pragma once

#include <unordered_map>

#include "Area.hpp"
#include "AreaMesh.hpp"
#include "Geometry.hpp"
#include "GeometryList.hpp"
#include "KeyPoint.hpp"
#include "Line.hpp"
#include "Mesh.hpp"
#include "MeshFactory.hpp"

namespace meshlib {

using namespace geomlib;

typedef Vertex EuclidianVertex;
typedef Vertex TopologicalVertex;

class MeshManager {
public:
    static MeshManager* getDefaultInstance();

    Vertex* meshVertex(KeyPoint* point);
    std::vector<Vertex*> meshLine(Line* line);
    AreaMesh meshArea(Area* area);

    void vinculateTopologicalVertex(TopologicalVertex* topological, EuclidianVertex* euclidian);
    EuclidianVertex* getEuclidianVertex(TopologicalVertex* topological);

    MeshManager(MeshManager const&) = delete;
    void operator=(MeshManager const&) = delete;
private:
    MeshManager() {}

    std::unordered_map<KeyPoint*, EuclidianVertex*> pointMap;
    std::unordered_map<Line*, std::vector<EuclidianVertex*>> lineMap;
    std::unordered_map<Area*, AreaMesh> areaMap;

    std::unordered_map<TopologicalVertex*, EuclidianVertex*> euclidianVertexMap;
};

}