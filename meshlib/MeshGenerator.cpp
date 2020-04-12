#include "MeshGenerator.hpp"
#include "Area.hpp"
#include "Vertex.hpp"
#include "meshlib.hpp"

namespace meshlib {

Mesh MeshGenerator::generateMesh() {
    Mesh mesh;

    // Generate Points Nodes
    auto points = geomList->getListOf(geomlib::GeometryType::Keypoint);
    for(auto point : points) {
        auto p = dynamic_cast<Point*>(point);
        auto vertex = new Vertex(*p);
        mesh.vertices.push_back(vertex);
        Mesh pointMesh;
        pointMesh.vertices.push_back(vertex);
        meshMap[point] = pointMesh;
    }

    // Generate Lines Nodes
    auto lines = geomList->getListOf(geomlib::GeometryType::Line);
    for(auto gLine : lines) {
        Mesh lineMesh;
        auto line = dynamic_cast<Line*>(gLine);
        auto initVertex = meshMap[line->init_point].vertices[0];
        auto finalVertex = meshMap[line->final_point].vertices[0];
        lineMesh.vertices.push_back(initVertex);
        double qtdElements = ceil(line->length() / elementSize);

        auto lineVertices = subdivideLine(line, qtdElements);
        for(auto v : lineVertices) {
            mesh.vertices.push_back(v);
            lineMesh.vertices.push_back(v);
        }
        lineMesh.vertices.push_back(finalVertex);

        meshMap[line] = lineMesh;
    }

    // Generate Area Nodes

    return mesh;
}

} // namespace meshlib