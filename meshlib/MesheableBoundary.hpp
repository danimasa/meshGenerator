#pragma once

#include <vector>

#include "Vertex.hpp"

namespace meshlib {

struct MesheableBoundary {
    std::vector<Vertex*> north;
    std::vector<Vertex*> south;
    std::vector<Vertex*> west;
    std::vector<Vertex*> east;
};

} // namespace meshlib
