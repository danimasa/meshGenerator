#pragma once

#include <vector>
#include "QuadArea.hpp"

namespace meshlib { 

using namespace geomlib;

class MeshShapes {
public:
    enum class RelativeShapes {
        POOO_0, POOO_90, POOO_180, POOO_270,
        PPOO_0, PPOO_90, PPOO_180, PPOO_270,
        POPO_0, POPO_90,
        POPP_0, POPP_90, POPP_180, POPP_270,
        PPPP,
        OOOO,
    };

    enum class VertexShape {
        ANY,
        ORTHOGONAL,
        POLAR
    };

    static std::vector<RelativeShapes> generateShapeList(const QuadArea& area);

private:
    MeshShapes () {}

}; // class MeshShapes

} // namespace meshlib