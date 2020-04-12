#pragma once

#include "Area.hpp"
#include "QuadArea.hpp"
#include "Mesh.hpp"

namespace meshlib {

using namespace geomlib;

class AreaMesh : public Area {
public:
    Mesh generateMesh();

private:
    std::vector<QuadArea*> subareas;
    double elementSize;
}; // class AreaMesh

} // namespace meshlib