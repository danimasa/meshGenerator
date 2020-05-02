#pragma once

#include <vector>

#include "Mesh.hpp"
#include "QuadArea.hpp"
#include "AreaMesh.hpp"
#include "MeshShapes.hpp"
#include "MesheableBoundary.hpp"

namespace meshlib {

//TODO Generate 2D Vertex
Mesh* generateRegGrid(int elements_x, int elements_y);

Mesh* transfiniteMapping(Mesh &parametricMesh, QuadArea &area);

MesheableBoundary generateMesheableBoundary(const QuadArea &area);

// std::vector<Vertex*> subdivideLine(Line* line, int qtdElements, double lastXPos = 0.0, double pPosition = 1.0);

double angleBetweenLines(const Line &l1, const Line &l2);

enum class APROX_DIRECTION { BOTH, UP, DOWN };

/* receives a int approximation and find the index and 
 *   value of the less error approximation change value
 *
 * values: pair of approximated values and original values
 * range: the range to variate the value for approximation
 * 
 * returns: index and new lessError approximation value
 */
std::pair<int, int> lessErrorApproximation(
    std::vector<std::pair<int, double>> values,
    APROX_DIRECTION direction
);

}