#include "AreaMesh.hpp"
#include "meshlib.hpp"
#include "Polyline.hpp"

#include <math.h>
#include <vector>
#include <functional>

namespace meshlib {

struct PolylineProcess {
    double Vertex::* e;
    double Vertex::* n;
    std::function<int(int, int, int)> vert_pos;
    int points_op_dir;
    int points_mesh_direction;
    LineDirection lDirection;
    LineDirection mDirection;
};

double posDir(double position, LineDirection direction) {
    return direction == LineDirection::DIRECT ? position : 1 - position;
}

void processPolyline(
    Mesh &regMesh,
    Polyline *l,
    vector<Vertex*> linePoints,
    PolylineProcess &params
) {
    auto inter_points = l->intermidiaryPoints();
    double lastPosition = 0;
    int lastIndex = 0;
    double Vertex::* e = params.e;
    double Vertex::* n = params.n;
    for(auto kp : inter_points) {
        double iPosition = l->isPointInLine(*kp);

        if (params.lDirection == LineDirection::INVERSE)
            iPosition = 1 - iPosition;

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
            double diff = std::fabs(iPosition - p->*e);
            if (pDiff == 0 || diff <= pDiff) {
                pDiff = diff;
                sDiff = iPosition - p->*e;
                pPosition = i;
            }
        }

        linePoints[pPosition]->*e = iPosition;
        // correcting other points
        double initPosition = linePoints[firstPoint - 1]->*e;
        double distance = (iPosition - initPosition) / (pPosition - firstPoint + 1);
        double location = initPosition + distance;
        for(int i = firstPoint; i < pPosition; i++) {
            linePoints[i]->*e = location;
            location += distance;
        }
        double finalPosition = linePoints[lastPoint]->*e;
        distance = (finalPosition - iPosition) / (lastPoint - pPosition);
        location = iPosition + distance;
        for(int i = pPosition + 1; i < lastPoint; i++) {
            linePoints[i]->*e = location;
            location += distance;
        }

        // propagate on axis
        for(int i = firstPoint; i < lastPoint; i++) {
            double diff = 0.0;
            double fPosition = linePoints[firstPoint - 1]->*e;
            double lPosition = linePoints[lastPoint]->*e;
            if (i == pPosition) diff = iPosition;
            else if (i < pPosition)
                diff = fPosition + (i - firstPoint + 1) * ((iPosition - fPosition) / (pPosition - firstPoint + 1));
            else
                diff = iPosition + (i - pPosition) * ((lPosition - iPosition) / (lastPoint - pPosition));
            for(int o = 1; o < params.points_op_dir; o++) {
                auto p = regMesh.vertices[params.vert_pos(params.points_mesh_direction, i, o)];
                double corr = params.mDirection == LineDirection::DIRECT ? (1 - p->*n) : p->*n;
                p->*e += (diff - p->*e) * corr;
            }
        }

        lastPosition = iPosition;
        lastIndex = pPosition;
    }
}

void processPolylines (Mesh &regMesh, QuadArea* area, int points_r, int points_s) {
    if (area->lines[0].line->getLineType() == LineType::Polyline) {
        auto l = area->lines[0];
        Polyline* cLine = dynamic_cast<Polyline*>(l.line);
        vector<Vertex*> linePoints;
        linePoints.reserve(points_r);
        for(int i = 0; i < points_r; i++)
            linePoints.push_back(regMesh.vertices[i * points_s]);

        PolylineProcess p {
            &Vertex::x,
            &Vertex::y,
            [](int points, int i, int o) { return points * i + o; },
            points_s,
            points_s,
            l.direction,
            LineDirection::DIRECT
        };
        
        processPolyline(regMesh, cLine, linePoints, p);
    }

    if(area->lines[1].line->getLineType() == LineType::Polyline) {
        auto l = area->lines[1];
        Polyline* cLine = dynamic_cast<Polyline*>(l.line);
        vector<Vertex*> linePoints;
        linePoints.reserve(points_s);
        for(int i = 0; i < points_s; i++)
            linePoints.push_back(regMesh.vertices[(points_r - 1) * points_s + i]);
        
        PolylineProcess p {
            &Vertex::y,
            &Vertex::x,
            [](int points, int i, int o) { return points * o + i; },
            points_r,
            points_s,
            l.direction,
            LineDirection::INVERSE
        };
        
        processPolyline(regMesh, cLine, linePoints, p);
    }

    if (area->lines[2].line->getLineType() == LineType::Polyline) {
        auto l = area->lines[2];
        Polyline* cLine = dynamic_cast<Polyline*>(l.line);
        vector<Vertex*> linePoints;
        linePoints.reserve(points_r);
        for(int i = 1; i < points_r + 1; i++)
            linePoints.push_back(regMesh.vertices[(i * points_s) - 1]);
        
        PolylineProcess p {
            &Vertex::x,
            &Vertex::y,
            [](int points, int i, int o) { return points * i + o; },
            points_s,
            points_s,
            l.direction,
            LineDirection::INVERSE
        };
        
        processPolyline(regMesh, cLine, linePoints, p);
    }

    if(area->lines[3].line->getLineType() == LineType::Polyline) {
        auto l = area->lines[3];
        Polyline* cLine = dynamic_cast<Polyline*>(l.line);
        vector<Vertex*> linePoints;
        linePoints.reserve(points_s);
        for(int i = 0; i < points_s; i++)
            linePoints.push_back(regMesh.vertices[i]);
        
        PolylineProcess p {
            &Vertex::y,
            &Vertex::x,
            [](int points, int i, int o) { return points * o + i; },
            points_r,
            points_s,
            l.direction,
            LineDirection::DIRECT
        };
        
        processPolyline(regMesh, cLine, linePoints, p);
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