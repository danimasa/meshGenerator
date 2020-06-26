#pragma once

#include "Area.hpp"
#include "QuadArea.hpp"
#include "Mesh.hpp"

namespace meshlib {

class AreaMesh : public geomlib::Area {
public:
    Mesh* generateMesh();

private:
    vector<QuadArea*> subareas;
    Mesh* mesh;
}; // class AreaMesh

} // namespace meshlib