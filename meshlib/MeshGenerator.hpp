#pragma once

#include "Mesh.hpp"
#include "Geometry.hpp"
#include "AreaMesh.hpp"

namespace meshlib
{

class MeshGenerator {
public:
    MeshGenerator(double elementSize)
        : elementSize(elementSize) {}

    void addGeometry(const geomlib::Geometry* geometry);
    Mesh generateMesh();

private:
    double elementSize;
    vector<AreaMesh*> geomList;
};
    
} // namespace meshlib
