#include "Geometry.hpp"
#include "GeometryFactory.hpp"

namespace geomlib {

Geometry::Geometry()
    : ID(0)
{
}

Geometry::Geometry(const Geometry &geometry)
    : ID(geometry.ID)
{
}

}