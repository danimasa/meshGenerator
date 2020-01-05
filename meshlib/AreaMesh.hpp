#pragma once

#include "QuadArea.hpp"
#include "Mesh.hpp"

namespace meshlib {

using namespace geomlib;

class AreaMesh {
public:
    AreaMesh(double elementSize)
        : elementSize(elementSize) {}

    Mesh* generateMesh(QuadArea* area);
private:
    double elementSize;
}; // class AreaMesh

} // namespace meshlib