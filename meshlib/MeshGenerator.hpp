#pragma once

#include <unordered_map>

#include "Mesh.hpp"
#include "Geometry.hpp"
#include "GeometryList.hpp"


namespace meshlib
{

class MeshGenerator {
public:
    MeshGenerator(
        geomlib::GeometryList* geomList, 
        double elementSize
    )   : elementSize(elementSize),
        geomList(geomList) {}

    Mesh generateMesh();

private:
    double elementSize;
    std::unordered_map<geomlib::Geometry*, Mesh> meshMap;
    geomlib::GeometryList* geomList;
};
    
} // namespace meshlib
