#include "AreaMesh.hpp"
#include "meshlib.hpp"

#include <math.h>
#include <vector>

namespace meshlib {

Mesh AreaMesh::generateMesh(QuadArea* area) {
    double major_r = area->lines[0].line->length() >= area->lines[2].line->length()
        ? area->lines[0].line->length()
        : area->lines[2].line->length();

    double major_s = area->lines[1].line->length() >= area->lines[3].line->length()
        ? area->lines[1].line->length()
        : area->lines[3].line->length();

    int qtd_elements_r = std::ceil(major_r / elementSize);
    int qtd_elements_s = std::ceil(major_s / elementSize);

    Mesh regMesh = generateRegGrid(qtd_elements_r, qtd_elements_s);
    return transfiniteMapping(regMesh, *area);
}

} // namespace meshlib