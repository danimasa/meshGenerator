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

Mesh transfiniteMapping(Mesh &parametricMesh, QuadArea &area);

MesheableBoundary generateMesheableBoundary(const AreaMesh &area);

std::vector<Vertex*> subdivideLine(Line* line, int qtdElements, double lastXPos = 0.0, double pPosition = 1.0);

double angleBetweenLines(const Line &l1, const Line &l2);

}