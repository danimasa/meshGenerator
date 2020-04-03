#include "MeshGenerator.hpp"
#include "Area.hpp"

namespace meshlib {

void MeshGenerator::addGeometry(const geomlib::Geometry* geometry) {
    switch (geometry->getGeometryType())
    {
    case GeometryType::Area:
        auto area = dynamic_cast<Area*>(geometry);
        auto areaMesh = new AreaMesh(area->loops[0]->lines, elementSize);
        geomList.push_back(areaMesh);
        break;
    
    default:
        throw "Not implemented geometry in MeshGenerator";
    }
}

Mesh MeshGenerator::generateMesh() {

}

} // namespace meshlib