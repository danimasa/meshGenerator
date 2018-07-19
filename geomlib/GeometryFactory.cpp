#include "GeometryFactory.hpp"

namespace geomlib {

const GeometryFactory* GeometryFactory::getDefaultInstance()
{
    static GeometryFactory instance;
    return &instance;
}

}