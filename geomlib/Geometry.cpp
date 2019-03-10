#include "Geometry.hpp"
#include "GeometryFactory.hpp"

namespace geomlib {

int Geometry::next_id = -1;
int Geometry::getNextId() {
    return next_id++;
}

Geometry::Geometry()
{
    ID = getNextId();
}

Geometry::Geometry(const Geometry &geometry)
    : ID(geometry.ID)
{
}

}