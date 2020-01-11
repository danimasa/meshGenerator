#pragma once

#include <string>
#include "Mesh.hpp"

using namespace std;
using namespace meshlib;

namespace gmshlib {

class MeshWriter {
public:
    MeshWriter(Mesh* mesh)
        : mesh(mesh) {}

    string getMshFile();

private:
    Mesh* mesh;
};

}