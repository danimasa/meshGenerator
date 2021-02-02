#pragma once

#include "Mesh.hpp"
#include "GeometryList.hpp"
#include "MeshFactory.hpp"
#include "AppParams.hpp"

namespace meshlib
{

class MeshGenerator {
public:
    MeshGenerator(
        geomlib::GeometryList* geomList, 
        geomlib::AppParams& params
        ) : geomList(geomList)
    {
        MeshFactory::init(params);
    }

    Mesh generateMesh();

private:
    geomlib::GeometryList* geomList;
};
    
} // namespace meshlib
