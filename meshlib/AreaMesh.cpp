#include "AreaMesh.hpp"

#include <math.h>
#include <vector>

namespace meshlib {

Mesh* AreaMesh::generateMesh(QuadArea* area) {
    double major_r = area->lines[0].line->length() >= area->lines[2].line->length()
        ? area->lines[0].line->length()
        : area->lines[2].line->length();

    double major_s = area->lines[1].line->length() >= area->lines[3].line->length()
        ? area->lines[1].line->length()
        : area->lines[3].line->length();

    int qtd_elements_r = std::ceil(major_r / elementSize);
    int qtd_elements_s = std::ceil(major_s / elementSize);
    double t_increment_r = 1.0 / qtd_elements_r;
    double t_increment_s = 1.0 / qtd_elements_s;

    std::vector<Point*> pts_line_0, pts_line_1, pts_line_2, pts_line_3;

    pts_line_0.reserve(qtd_elements_r + 1);
    pts_line_1.reserve(qtd_elements_s + 1);
    pts_line_2.reserve(qtd_elements_r + 1);
    pts_line_3.reserve(qtd_elements_s + 1);

    auto line_0 = area->lines[0];
    auto line_1 = area->lines[1];
    auto line_2 = area->lines[2];
    auto line_3 = area->lines[3];

    double t_position = 0;
    for(int i = 0; i < qtd_elements_r - 1; i++) {
        t_position += t_increment_r;

        double p0_pos = line_0.direction == LineDirection::DIRECT
            ? t_position : 1 - t_position;
        double p2_pos = line_2.direction == LineDirection::DIRECT
            ? t_position : 1 - t_position;

        auto p0 = new Point(line_0.line->pointAtPosition(p0_pos));
        auto p2 = new Point(line_2.line->pointAtPosition(p2_pos));

        pts_line_0.push_back(p0);
        pts_line_2.push_back(p2);
    }

    t_position = 0;
    for(int i = 0; i < qtd_elements_s - 1; i++) {
        t_position += t_increment_s;

        double p1_pos = line_1.direction == LineDirection::DIRECT
            ? t_position : 1 - t_position;
        double p3_pos = line_3.direction == LineDirection::DIRECT
            ? t_position : 1 - t_position;

        auto p1 = new Point(line_1.line->pointAtPosition(p1_pos));
        auto p3 = new Point(line_3.line->pointAtPosition(p3_pos));

        pts_line_1.push_back(p1);
        pts_line_3.push_back(p3);
    }

    return new Mesh();
}

} // namespace meshlib