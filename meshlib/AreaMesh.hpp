#pragma once

#include "Area.hpp"
#include "Mesh.hpp"

namespace meshlib {

using namespace geomlib;

class AreaMesh {
public:
    AreaMesh(Area *area, double elementSize)
        : area(area),
        elementSize(elementSize) {}

    Mesh generateMesh();

private:
    double elementSize;
    Area* area;
    QuadArea* quadArea;
}; // class AreaMesh

} // namespace meshlib