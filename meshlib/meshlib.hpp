#pragma once

#include "Mesh.hpp"
#include "QuadArea.hpp"

namespace meshlib {

//TODO Generate 2D Vertex
Mesh generateRegGrid(int elements_x, int elements_y);

Mesh transfiniteMapping(Mesh &parametricMesh, geomlib::QuadArea &area);

}