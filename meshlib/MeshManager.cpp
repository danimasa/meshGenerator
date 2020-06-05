#include "MeshManager.hpp"

#include "meshlib.hpp"
#include "Polyline.hpp"

namespace meshlib {

MeshManager* MeshManager::getDefaultInstance() {
    static MeshManager instance;
    return &instance;
}

Vertex* MeshManager::meshVertex(KeyPoint* point) {
    if (pointMap.find(point) != pointMap.end())
        return pointMap[point];

    auto factory = MeshFactory::getDefaultInstance();
    auto vertex = factory->genVertex(point);
    pointMap[point] = vertex;
    return vertex;
}

std::vector<Vertex*> MeshManager::meshLine(Line* line) {
    if (lineMap.find(line) != lineMap.end())
        return lineMap[line];

    std::vector<Vertex*> lineVertices;
    auto initVertex = meshVertex(line->init_point);
    auto finalVertex = meshVertex(line->final_point);
    lineVertices.push_back(initVertex);

    auto factory = MeshFactory::getDefaultInstance();
    vector<Vertex*> innerVertices;
    if(line->getLineType() == LineType::Polyline) {
        auto pLine = dynamic_cast<Polyline*>(line);
        auto iPoints = pLine->intermidiaryPoints();
        int iPindex = 0;
        for(auto l : pLine->getLinesWithDirection()) {
            auto subVertices = meshLine(l.line);
            if(l.direction == LineDirection::INVERSE)
                std::reverse(subVertices.begin(), subVertices.end());
            
            innerVertices.insert(
                innerVertices.end(),
                subVertices.begin() + 1,
                subVertices.end() - 1
            );

            if(iPindex < iPoints.size()) {
                auto iKp = iPoints[iPindex];
                innerVertices.push_back(meshVertex(iKp));
            }
            iPindex++;
        }
    } else {
        innerVertices = factory->generateLineElements(line);
    }

    for(auto v : innerVertices)
        lineVertices.push_back(v);

    lineVertices.push_back(finalVertex);

    lineMap[line] = lineVertices;
    return lineVertices;
}

AreaMesh MeshManager::meshArea(Area* area) {
    if (areaMap.find(area) != areaMap.end())
        return areaMap[area];

    if(area->loops.size() > 1) throw "AreaMesh: Not implemented internal loops in area";
    if(area->loops[0]->lines.size() != 4) throw "AreaMesh: Only quadrilateral areas implemented";

    auto factory = MeshFactory::getDefaultInstance();
    auto qArea = factory->subdivideArea(area);

    auto mesh = factory->genSurfaceMesh(qArea);
    AreaMesh aMesh;
    aMesh.addMesh(qArea, mesh);

    return aMesh;
}

void MeshManager::vinculateTopologicalVertex(TopologicalVertex* topological, EuclidianVertex* euclidian) {
    euclidianVertexMap.insert({ topological, euclidian });
}

EuclidianVertex* MeshManager::getEuclidianVertex(TopologicalVertex* topological) {
    return euclidianVertexMap[topological];
}

}