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

}