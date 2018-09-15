#include <algorithm>
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

std::vector<Geometry*> GeometryList::getListOf(std::string geometryType) {
    std::vector<Geometry*> result;
    for(auto i = 0; i < objects.size(); i++) {
        auto obj = objects.at(i);
        if (obj->getGeometryType() == geometryType) {
            result.push_back(obj);
        }
    }
    return result;
}

Geometry* GeometryList::getByID(std::string geometryType, int id) {
    auto geomList = getListOf(geometryType);
    auto it = std::find_if(geomList.begin(), geomList.end(), [&](const Geometry* o) -> bool {
        return o->getID() == id;
    });
    return *it;
}

} // geomlib