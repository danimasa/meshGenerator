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
        double lastPosition = 0;
        int lastIndex = 0;
        for(auto kp : inter_points) {
            double iPosition = cLine->isPointInLine(*kp);
            int firstPoint = 0;
            int lastPoint = linePoints.size() - 1;
            if (lastPosition < iPosition) {
                firstPoint = lastIndex + 1;
            } else {
                lastPoint = lastIndex - 1;
            }
            
            double pDiff = 0;
            double sDiff = 0;
            int pPosition = 0;
            // finding nearer point
            for(int i = firstPoint; i < lastPoint; i++) {
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
            double initPosition = linePoints[firstPoint - 1]->x;
            double distance = (iPosition - initPosition) / (pPosition - firstPoint + 1);
            double location = initPosition + distance;
            for(int i = firstPoint; i < pPosition; i++) {
                linePoints[i]->x = location;
                location += distance;
            }
            double finalPosition = linePoints[lastPoint]->x;
            distance = (finalPosition - iPosition) / (lastPoint - pPosition);
            location = iPosition + distance;
            for(int i = pPosition + 1; i < lastPoint; i++) {
                linePoints[i]->x = location;
                location += distance;
            }

            // propagate on axis
            for(int i = firstPoint; i < lastPoint; i++) {
                double diff = 0.0;
                double fPosition = linePoints[firstPoint - 1]->x;
                double lPosition = linePoints[lastPoint]->x;
                if (i == pPosition) diff = iPosition;
                else if (i < pPosition)
                    diff = fPosition + (i - firstPoint + 1) * ((iPosition - fPosition) / (pPosition - firstPoint + 1));
                else
                    diff = iPosition + (i - pPosition) * ((lPosition - iPosition) / (lastPoint - pPosition));
                for(int o = 1; o < points_s; o++) {
                    auto p = regMesh.vertices[points_s * i + o];
                    p->x += (diff - p->x) * (1 - p->y);
                }
            }

            lastPosition = iPosition;
            lastIndex = pPosition;
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