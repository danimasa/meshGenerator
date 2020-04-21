#pragma once

#include "QuadArea.hpp"
#include "Mesh.hpp"

namespace meshlib {

class AreaMesh {
public:
    void addMesh(QuadArea *area, Mesh *mesh);

    QuadArea* subareas;
    Mesh* mesh;
}; // class AreaMesh

} // namespace meshlib