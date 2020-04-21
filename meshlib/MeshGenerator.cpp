#include "MeshGenerator.hpp"
#include "Area.hpp"
#include "Vertex.hpp"
#include "meshlib.hpp"
#include "MeshManager.hpp"

namespace meshlib {

Mesh MeshGenerator::generateMesh() {
    Mesh mesh;
    auto manager = MeshManager::getDefaultInstance();

    // Generate Points Nodes
    auto points = geomList->getListOf(geomlib::GeometryType::Keypoint);
    for(auto point : points) {
        auto p = dynamic_cast<KeyPoint*>(point);
        auto vertex = manager->meshVertex(p);
        mesh.addVertex(vertex);
    }

    // Generate Lines Nodes
    auto lines = geomList->getListOf(geomlib::GeometryType::Line);
    for(auto gLine : lines) {
        auto line = dynamic_cast<Line*>(gLine);
        auto vertices = manager->meshLine(line);
        mesh.addVertices(vertices);
    }

    // Generate Area Elements
    auto areas = geomList->getListOf(geomlib::GeometryType::Area);
    for(auto gArea : areas) {
        auto area = dynamic_cast<Area*>(gArea);
        auto aMesh = manager->meshArea(area);
        mesh.mergeMesh(aMesh.mesh);
    }

    return mesh;
}

} // namespace meshlib