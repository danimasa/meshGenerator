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
        OOOO, OOOO_C,
        OOOO_L_0, OOOO_L_90, OOOO_L_180, OOOO_L_270,
        AOOOP_0, AOOOP_90, AOOOP_180, AOOOP_270
    };

    enum class VertexShape {
        ANY,
        ORTHOGONAL,
        POLAR
    };

    struct ElementVertexShapes {
        VertexShape v1s;
        VertexShape v2s;
        VertexShape v3s;
        VertexShape v4s;
    };

    static std::vector<RelativeShapes> generateShapeList(const QuadArea& area);

private:
    MeshShapes () {}

    static std::vector<RelativeShapes> adjustShapeList(std::vector<RelativeShapes> &shapes);

}; // class MeshShapes

} // namespace meshlib