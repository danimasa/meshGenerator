#include <stdexcept>
#include <algorithm>

#include "meshlib.hpp"
#include "ArcLine.hpp"
#include "Polyline.hpp"
#include "MeshManager.hpp"

namespace meshlib {

//TODO Generate 2D Vertex
Mesh* generateRegGrid(int elements_x, int elements_y) {
    auto mesh = new Mesh();

    // mesh.vertices.reserve((elements_x + 1) * (elements_y + 1));
    // mesh.elements.reserve(elements_x * elements_y);

    double x_increment = 1.0 / elements_x;
    double y_increment = 1.0 / elements_y;

    double x_pos = 0.0;
    int fp_index = 0; // index of first point for element generation
    int cIndex = 0; // current Index
    for(int i = 0; i <= elements_x; i++) {
        double y_pos = 0.0;
        for(int j = 0; j <= elements_y; j++) {
            if (j == elements_y) y_pos = 1.0;
            if (i == elements_x) x_pos = 1.0;

            Point p { x_pos, y_pos, 0 };
            auto v = new Vertex(p);
            mesh->addVertex(v);

            if (i > 0 && j > 0) {
                if (fp_index == 0) fp_index = cIndex;
                auto v1 = mesh->getVertices()[cIndex - fp_index];
                auto v2 = mesh->getVertices()[cIndex - 1];
                auto v3 = v;
                auto v4 = mesh->getVertices()[cIndex - fp_index + 1];
                auto elem = new Quadrilateral{ v1, v2, v3, v4 };
                mesh->addElement(elem);
            }

            y_pos += y_increment;
            cIndex++;
        }
        x_pos += x_increment;
    }

    return mesh;
}

Mesh* transfiniteMapping(Mesh &parametricMesh, QuadArea &area) {
    auto mesh = new Mesh();
    mesh->addElements(parametricMesh.getElements());
    auto manager = MeshManager::getDefaultInstance();

    for(auto node : parametricMesh.getVertices()) {
        auto v = manager->getEuclidianVertex(node);
        if(!v) {
            v = new Vertex();

            // parameters
            auto l1 = area.lines[0];
            auto l2 = area.lines[1];
            auto l3 = area.lines[2];
            auto l4 = area.lines[3];

            double e = node->x;
            double n = node->y;

            auto c1 = l1.direction == LineDirection::DIRECT
                ? l1.line->pointAtPosition(e)
                : l1.line->pointAtPosition(1 - e);

            auto c2 = l2.direction == LineDirection::DIRECT
                ? l2.line->pointAtPosition(n)
                : l2.line->pointAtPosition(1 - n);

            auto c3 = l3.direction == LineDirection::DIRECT
                ? l3.line->pointAtPosition(e)
                : l3.line->pointAtPosition(1 - e);
            
            auto c4 = l4.direction == LineDirection::DIRECT
                ? l4.line->pointAtPosition(n)
                : l4.line->pointAtPosition(1 - n);

            // area vextex
            auto vertex = area.getVertex();
            auto v1 = vertex[0];
            auto v2 = vertex[1];
            auto v3 = vertex[2];
            auto v4 = vertex[3];

            double x_value = (1 - n) * c1.x + e * c2.x + n * c3.x + (1 - e) * c4.x
                - ((1 - e) * (1 - n) * v1->x + e * (1 - n) * v2->x + e * n * v3->x + (1 - e) * n * v4->x);

            double y_value = (1 - n) * c1.y + e * c2.y + n * c3.y + (1 - e) * c4.y
                - ((1 - e) * (1 - n) * v1->y + e * (1 - n) * v2->y + e * n * v3->y + (1 - e) * n * v4->y);

            double z_value = (1 - n) * c1.z + e * c2.z + n * c3.z + (1 - e) * c4.z
                - ((1 - e) * (1 - n) * v1->z + e * (1 - n) * v2->z + e * n * v3->z + (1 - e) * n * v4->z);

            v->x = x_value;
            v->y = y_value;
            v->z = z_value;
        }

        mesh->addVertex(v);

        // replace in elements
        for(auto elem : mesh->getElements()) {
            if(elem->v1 == node) elem->v1 = v;
            if(elem->v2 == node) elem->v2 = v;
            if(elem->v3 == node) elem->v3 = v;
            if(elem->v4 == node) elem->v4 = v;
        }
    }

    return mesh;
}

std::vector<Vertex*> fillBoundary(QuadArea::TopologicalLine &tLine, double Vertex::* x, double Vertex::* y, double yValue) {
    std::vector<Vertex*> vList;
    auto line = tLine.line;
    bool direct = tLine.direction == LineDirection::DIRECT;

    auto manager = MeshManager::getDefaultInstance();
    auto euclidianVertices = manager->meshLine(line);
    for(auto eucV : euclidianVertices) {
        auto v = new Vertex();
        double position = line->isPointInLine(*eucV);
        v->*x = direct ? position : 1 - position;
        v->*y = yValue;
        vList.push_back(v);
    }

    for(int i = 0; i < euclidianVertices.size(); i++) {
        manager->vinculateTopologicalVertex(vList[i], euclidianVertices[i]);
    }

    if (!direct) {
        std::reverse(vList.begin(), vList.end());
    }

    return vList;
}

MesheableBoundary generateMesheableBoundary(const QuadArea &area) {
    MesheableBoundary boundary;

    auto southLine = area.south();
    boundary.south = fillBoundary(southLine, &Vertex::x, &Vertex::y, 0.0);

    auto eastLine = area.east();
    boundary.east = fillBoundary(eastLine, &Vertex::y, &Vertex::x, 1.0);

    auto westLine = area.west();
    boundary.west = fillBoundary(westLine, &Vertex::y, &Vertex::x, 0.0);

    auto northLine = area.north();
    boundary.north = fillBoundary(northLine, &Vertex::x, &Vertex::y, 1.0);

    return boundary;
}

double angleBetweenLines(const Line &l1, const Line &l2) {
    // find common point
    Point *common_p = nullptr;
    if (l1.init_point == l2.init_point || l1.init_point == l2.final_point)
        common_p = l1.init_point;
    else if (l1.final_point == l2.init_point || l1.final_point == l2.final_point)
        common_p = l1.final_point;
    else throw std::invalid_argument("angleBetweenLines: Lines have to share a point");

    // find vector for the first line
    auto v1 = l1.init_point == common_p ? l1.get_init_vector().invert() : l1.get_final_vector();

    // find vector for the second line
    auto v2 = l2.init_point == common_p ? l2.get_init_vector().invert() : l2.get_final_vector();

    return v1.angleWith(v2);
}

}