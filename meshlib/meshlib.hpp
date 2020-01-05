#pragma once

#include "Mesh.hpp"
#include "QuadArea.hpp"

namespace meshlib {

//TODO Generate 2D Vertex
Mesh generateRegGrid(int elements_x, int elements_y) {
    Mesh mesh;

    mesh.vertices.reserve((elements_x + 1) * (elements_y + 1));
    mesh.elements.reserve(elements_x * elements_y);

    double x_increment = 1.0 / elements_x;
    double y_increment = 1.0 / elements_y;

    double x_pos = 0.0;
    int fp_index = 0; // index of first point
    int cIndex = 0; // current Index
    for(int i = 0; i <= elements_x; i++) {
        double y_pos = 0.0;
        for(int j = 0; j <= elements_y; j++) {
            if (j == elements_y) y_pos = 1.0;
            if (i == elements_x) x_pos = 1.0;

            auto v = new Vertex { x_pos, y_pos, 0 };
            mesh.vertices.push_back(v);

            if (i > 0 && j > 0) {
                if (fp_index == 0) fp_index = cIndex;
                auto v1 = mesh.vertices[cIndex - fp_index];
                auto v2 = mesh.vertices[cIndex - 1];
                auto v3 = v;
                auto v4 = mesh.vertices[cIndex - fp_index + 1];
                auto elem = new Quadrilateral{ v1, v2, v3, v4 };
                mesh.elements.push_back(elem);
            }

            y_pos += y_increment;
            cIndex++;
        }
        x_pos += x_increment;
    }

    return mesh;
}

Mesh transfiniteMapping(Mesh &parametricMesh, geomlib::QuadArea &area) {
    Mesh mesh;

    return mesh;
}

}