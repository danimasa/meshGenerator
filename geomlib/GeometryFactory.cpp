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

Line* GeometryFactory::createLine(int init_point_id, int final_point_id) const {
    return new Line(init_point_id, final_point_id);
}

}