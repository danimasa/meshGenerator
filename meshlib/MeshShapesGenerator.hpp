#pragma once

#include <vector>

#include "Mesh.hpp"
#include "MeshShapes.hpp"
#include "MesheableBoundary.hpp"
#include "QuadArea.hpp"

namespace meshlib {

class MeshShapesGenerator {
public:
    Mesh genMesh(std::vector<MeshShapes::RelativeShapes> shapeList, const QuadArea &area);

private:
    MesheableBoundary cBoundary;
    Mesh cMesh;

    void gen_POOO_0();
    void gen_POOO_90();
    void gen_POOO_180();
    void gen_POOO_270();

    void gen_PPOO_0();
    void gen_PPOO_90();
    void gen_PPOO_180();
    void gen_PPOO_270();

    void gen_POPO_0();
    void gen_POPO_90();

    void gen_POPP_0();
    void gen_POPP_90();
    void gen_POPP_180();
    void gen_POPP_270();

    void gen_PPPP();
    void gen_OOOO();
    void gen_OOOO_C();

    void gen_OOOO_L_0();
    void gen_OOOO_L_90();
    void gen_OOOO_L_180();
    void gen_OOOO_L_270();

    void generateElements (vector<Vertex*> lastVLine, vector<Vertex*> currVLine);
};

} // namespace meshlib