#pragma once

#include "Area.hpp"
#include "Mesh.hpp"

namespace meshlib {

using namespace geomlib;

class AreaMesh {
public:
    AreaMesh(Area* area)
        : area(area) {}

    Mesh* generateMesh();
private:
    Area *area;
}; // class AreaMesh

} // namespace meshlib