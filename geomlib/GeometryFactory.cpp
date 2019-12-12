#define _USE_MATH_DEFINES
#include "GeometryFactory.hpp"
#include <assert.h>
#include <algorithm>
#include <stdexcept>
#include <list>
#include "armadillo"

namespace geomlib {

const GeometryFactory* GeometryFactory::getDefaultInstance()
{
    static GeometryFactory instance;
    return &instance;
}

KeyPoint* GeometryFactory::createKeypoint() const {
    return new KeyPoint();
}

KeyPoint* GeometryFactory::createKeypoint(Point &point) const {
    return new KeyPoint(point);
}

StraightLine* GeometryFactory::createStraightLine(KeyPoint* init_point, KeyPoint* final_point) const {
    return new StraightLine(init_point, final_point);
}

ArcLine* GeometryFactory::createArcLine(KeyPoint* init_point, KeyPoint* final_point, Plane* plane,
        double radius, Point* center) const {
    return new ArcLine(plane, init_point, final_point, radius, center, false);
}

ArcLine* GeometryFactory::createArcLine(KeyPoint* init_point, KeyPoint* final_point, Point* mid_point,
        Vector* init_tangent_vector, Vector* final_tangent_vector) const {
    Plane* plane = createPlane(init_point, final_point, mid_point);
    auto planeNormal = plane->normalVector();
    Vector toCenter1 = init_tangent_vector->vectorProduct(planeNormal);
    Vector toCenter2 = final_tangent_vector->vectorProduct(planeNormal);
    
    // Calcula coeficientes alfa e beta
    arma::mat A;
    A << toCenter1.x << (toCenter2.x) * -1 << arma::endr
      << toCenter1.y << (toCenter2.y) * -1 << arma::endr;

    arma::vec b;
    b << final_point->x - init_point->x << final_point->y - init_point->y;

    arma::mat alphaBeta = arma::solve(A, b);

    // Validar coeficiente
    double alfa = alphaBeta(0);
    double beta = alphaBeta(1);
    // assert(alfa * toCenter1.z - beta * toCenter2.z != final_point->z - init_point->z);

    double xc = init_point->x + alfa * toCenter1.x;
    double yc = init_point->y + alfa * toCenter1.y;
    double zc = init_point->z + alfa * toCenter1.z;
    auto center = new Point(xc, yc, zc);

    double radius = init_point->distance(center);

    // Inverted Direction (Only anticlockwise is permited)
    Vector v1(center, init_point);
    auto v3 = v1.vectorProduct(plane->normalVector());

    auto arcInitPoint = init_point;
    auto arcFinalPoint = final_point;
    bool inverted = false;

    double angle = v3.angleWith(*init_tangent_vector);
    if(v3.angleWith(*init_tangent_vector) > M_PI / 2) {
        arcInitPoint = final_point;
        arcFinalPoint = init_point;
        inverted = true;
    }

    return new ArcLine(plane, arcInitPoint, arcFinalPoint, radius, center, inverted);
}

UnspecifiedLine* GeometryFactory::createUnspecifiedLine(
    KeyPoint* init_point,
    KeyPoint* final_point,
    Point* mid_point,
    Vector* tangent_init_vector,
    Vector* tangent_final_vector,
    double length) const {
    
    return new UnspecifiedLine(init_point, final_point, mid_point, tangent_init_vector, tangent_final_vector, length);
}

Polyline* GeometryFactory::createPolyline(
    KeyPoint* init_point,
    KeyPoint* final_point,
    std::vector<Line*> lines) const {
    
    return new Polyline(init_point, final_point, lines);
}

Plane* GeometryFactory::createPlane(Point* p1, Vector* v1, Vector* v2) const {
    return new Plane(p1, v1, v2);
}

Plane* GeometryFactory::createPlane(Point* p1, Point* p2, Point* p3) const {
    auto vector1 = new Vector(p1, p2);
    auto vector2 = new Vector(p1, p3);
    return new Plane(p1, vector1, vector2);
}

void verifyAreaLoop(const Area::Loop* loop) {
    if(loop->lines.size() < 3) {
        throw std::invalid_argument("An area loop should have a minimal of 3 lines");
    }

    auto first_line = loop->lines[0].line;
    KeyPoint *initPoint = first_line->init_point;
    KeyPoint *searchedPoint = first_line->final_point;

    auto lines = loop->lines;
    // Verify all touching lines
    std::vector<Line*> remainingLines = loop->getLines();

    // Remove first_line
    std::vector<Line*>::iterator firstIt = std::find(remainingLines.begin(), remainingLines.end(), first_line);
    remainingLines.erase(firstIt);

    do {
        bool findOne = false;
        for(auto it = remainingLines.begin(); it != remainingLines.end(); ++it) {
            auto line = *it;
            if(line->init_point == searchedPoint || line->final_point == searchedPoint) {
                if(line->init_point == searchedPoint)
                    searchedPoint = line->final_point;
                else
                    searchedPoint = line->init_point;
                remainingLines.erase(it);
                findOne = true;
                break;
            }
        }
        if(!findOne) {
            std::cout << "searchedPoint: " << searchedPoint->getID() << std::endl;
            throw std::invalid_argument("Not permited non touching lines");
        }
    } while(searchedPoint != initPoint && remainingLines.size() != 0);
}

Area* GeometryFactory::createArea(vector<Area::Loop*> loops) const {
    for(auto loop : loops) {
        verifyAreaLoop(loop);
    }

    return new Area(loops);
}

}