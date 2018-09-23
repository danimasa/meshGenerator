#include "GeometryFactory.hpp"
#include <assert.h>
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

    double radius = init_point->distance(*center);

    return new ArcLine(plane, init_point, final_point, radius, center);
}

UnspecifiedLine* GeometryFactory::createUnspecifiedLine(
    KeyPoint* init_point,
    KeyPoint* final_point,
    Point* mid_point,
    Vector* tangent_init_vector,
    Vector* tangent_final_vector) const {
    
    return new UnspecifiedLine(init_point, final_point, mid_point, tangent_init_vector, tangent_final_vector);
}

Plane* GeometryFactory::createPlane(Point* p1, Vector* v1, Vector* v2) const {
    return new Plane(p1, v1, v2);
}

Plane* GeometryFactory::createPlane(Point* p1, Point* p2, Point* p3) const {
    auto vector1 = new Vector(p1, p2);
    auto vector2 = new Vector(p1, p3);
    return new Plane(p1, vector1, vector2);
}

}