#pragma once

#include <vector>

#include "Mesh.hpp"
#include "QuadArea.hpp"
#include "AreaMesh.hpp"
#include "MeshShapes.hpp"
#include "MesheableBoundary.hpp"

namespace meshlib {

//TODO Generate 2D Vertex
Mesh generateRegGrid(int elements_x, int elements_y);

Mesh transfiniteMapping(Mesh &parametricMesh, geomlib::QuadArea &area);

MesheableBoundary generateMesheableBoundary(const AreaMesh &area);

}