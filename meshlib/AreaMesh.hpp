#pragma once

#include "QuadArea.hpp"
#include "Mesh.hpp"

namespace meshlib {

using namespace geomlib;

class AreaMesh : public QuadArea {
public:
    AreaMesh(vector<Line*> lines, double elementSize)
        : elementSize(elementSize),
        QuadArea(lines) {
            determineLinesSubdivision();
        }

    AreaMesh(QuadArea &area, double elementSize)
        : elementSize(elementSize),
        QuadArea(area) {
            determineLinesSubdivision();
        }

    Mesh generateMesh();

private:
    double elementSize;

    void determineLinesSubdivision();
}; // class AreaMesh

} // namespace meshlib