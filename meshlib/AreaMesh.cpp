#include "AreaMesh.hpp"
#include "meshlib.hpp"
#include "Polyline.hpp"
#include "MeshShapesGenerator.hpp"
#include "MeshShapes.hpp"
#include "QuadArea.hpp"

#include <math.h>
#include <vector>
#include <functional>

namespace meshlib {

void AreaMesh::addMesh(QuadArea* area, Mesh *mesh) {
    this->subareas = area;
    this->mesh = mesh;
}

} // namespace meshlib