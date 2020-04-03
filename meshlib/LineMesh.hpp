#pragma once

#include <vector>

#include "Line.hpp"
#include "KeyPointMesh.hpp"
#include "Vertex.hpp"

namespace meshlib {

class LineMesh {
public:
    geomlib::Line* line;
    std::vector<Vertex*> euclidian_points;
    std::vector<Vertex*> topoligical_points;

    KeyPointMesh* init_point;
    KeyPointMesh* final_point;
};

} // namespace meshlib