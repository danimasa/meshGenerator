#include "GeometryList.hpp"

namespace geomlib {

int GeometryList::size() const {
    return objects.size();
}

int GeometryList::capacity() const {
    return objects.capacity();
}

void GeometryList::reserveSize(int length) {
    objects.reserve(length);
}

void GeometryList::add(Geometry* geometry) {
    objects.push_back(geometry);
}

}