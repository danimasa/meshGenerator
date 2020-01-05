#pragma once
#include <vector>

#include "Point.hpp"

namespace meshlib
{
    
using namespace geomlib;

typedef geomlib::Point Vertex;

struct Quadrilateral {
    Vertex *v1, *v2, *v3, *v4;
};

struct Mesh {
    std::vector<Vertex*> vertices;
    std::vector<Quadrilateral*> elements;
};

} // namespace meshlib
