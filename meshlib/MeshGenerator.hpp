#pragma once

#include "Mesh.hpp"
#include "GeometryList.hpp"
#include "MeshFactory.hpp"

namespace meshlib
{

class MeshGenerator {
public:
    MeshGenerator(
        geomlib::GeometryList* geomList, 
        double elementSize
    )   : elementSize(elementSize),
        geomList(geomList) {
        
        auto factory = MeshFactory::getDefaultInstance();
        factory->setElementSize(elementSize);
    }

    Mesh generateMesh();

private:
    double elementSize;
    geomlib::GeometryList* geomList;
};
    
} // namespace meshlib
