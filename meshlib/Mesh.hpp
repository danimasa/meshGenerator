#pragma once
#include <vector>

#include "Vertex.hpp"
#include "Quadrilateral.hpp"

namespace meshlib
{

struct Mesh {
    std::vector<Vertex*> vertices;
    std::vector<Quadrilateral*> elements;
};

} // namespace meshlib
