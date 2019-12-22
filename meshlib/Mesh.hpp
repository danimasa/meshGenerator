#pragma once
#include <vector>

#include "Point.hpp"

namespace meshlib
{
    
using namespace geomlib;

struct Quadrilateral {
    std::vector<Point*> vertices;
};

struct Mesh {
    std::vector<Point*> vertices;
    std::vector<Quadrilateral*> elements;
};

} // namespace meshlib
