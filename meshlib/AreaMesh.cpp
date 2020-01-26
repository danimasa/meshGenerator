#include "AreaMesh.hpp"
#include "meshlib.hpp"
#include "Polyline.hpp"

#include <math.h>
#include <vector>

namespace meshlib {

void processPolylines (Mesh &regMesh, QuadArea* area, int points_r, int points_s) {
    if (area->lines[0].line->getLineType() == LineType::Polyline) {
        Polyline* cLine = dynamic_cast<Polyline*>(area->lines[0].line);
        vector<Vertex*> linePoints;
        linePoints.reserve(points_r);
        for(int i = 0; i < points_r; i++)
            linePoints.push_back(regMesh.vertices[i * points_s]);
        
        auto inter_points = cLine->intermidiaryPoints();
        for(auto kp : inter_points) {
            double iPosition = cLine->isPointInLine(*kp); 
            double pDiff = 0;
            double sDiff = 0;
            int pPosition = 0;
            // finding nearer point
            for(int i = 1; i < linePoints.size() - 1; i++) {
                auto p = linePoints[i];
                double diff = std::fabs(iPosition - p->x);
                if (pDiff == 0 || diff <= pDiff) {
                    pDiff = diff;
                    sDiff = iPosition - p->x;
                    pPosition = i;
                }
            }

            linePoints[pPosition]->x = iPosition;
            // correcting other points
            double distance = iPosition / pPosition;
            double location = distance;
            for(int i = 1; i < pPosition; i++) {
                linePoints[i]->x = location;
                location += distance;
            }
            distance = (1 - iPosition) / (linePoints.size() - pPosition - 1);
            location = iPosition + distance;
            for(int i = pPosition + 1; i < linePoints.size() - 1; i++) {
                linePoints[i]->x = location;
                location += distance;
            }

            // propagate on axis
            for(int i = 1; i < points_r - 1; i++) {
                for(int o = 1; o < points_s; o++) {
                    auto p = regMesh.vertices[points_s * i + o];
                    p->x += sDiff * (1 - p->y);
                }
            }
        }
    }
}

Mesh AreaMesh::generateMesh(QuadArea* area) {
    double major_r = area->lines[0].line->length() >= area->lines[2].line->length()
        ? area->lines[0].line->length()
        : area->lines[2].line->length();

    double major_s = area->lines[1].line->length() >= area->lines[3].line->length()
        ? area->lines[1].line->length()
        : area->lines[3].line->length();

    int qtd_elements_r = std::ceil(major_r / elementSize);
    int qtd_elements_s = std::ceil(major_s / elementSize);
    int points_r = qtd_elements_r + 1;
    int points_s = qtd_elements_s + 1;

    Mesh regMesh = generateRegGrid(qtd_elements_r, qtd_elements_s);
    processPolylines(regMesh, area, points_r, points_s);
    return transfiniteMapping(regMesh, *area);
}

} // namespace meshlib