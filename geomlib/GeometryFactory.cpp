#include "GeometryFactory.hpp"

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

UnspecifiedLine* GeometryFactory::createUnspecifiedLine(
    KeyPoint* init_point,
    KeyPoint* final_point,
    Point* mid_point,
    Vector* tangent_init_vector,
    Vector* tangent_final_vector) const {
    
    return new UnspecifiedLine(init_point, final_point, mid_point, tangent_init_vector, tangent_final_vector);
}

}