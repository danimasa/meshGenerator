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

Mesh AreaMesh::generateMesh() {
    if(loops.size() > 1) throw "AreaMesh: Not implemented internal loops in area";
    if(loops[0]->lines.size() != 4) throw "AreaMesh: Only quadrilateral areas implemented";

    QuadArea qArea(loops[0]->lines);
    // qArea.determineLinesSubdivision(elementSize);
    auto shapeList = MeshShapes::generateShapeList(qArea);
    MeshShapesGenerator gen;
    auto regMesh = gen.genMesh(shapeList, *this);
    // return regMesh;
    return transfiniteMapping(regMesh, qArea);
}

} // namespace meshlib