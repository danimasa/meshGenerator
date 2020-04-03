#pragma once

#include "KeyPoint.hpp"
#include "Vertex.hpp"

namespace meshlib {

class KeyPointMesh {
public:
    geomlib::KeyPoint point;
    Vertex* vertex;
};

} // namespace meshlib