#include <algorithm>
#include <stdexcept>
#include <iterator>
#include "GeometryList.hpp"
#include "Area.hpp"

namespace geomlib {

int GeometryList::size() const {
    return objects.size();
}

int GeometryList::capacity() const {
    return objects.capacity();
}

void GeometryList::reserveSize(int length) {
    objects.reserve(length);
    ids.reserve(length);
}

void GeometryList::substituteInAreas(Line *line) {
  auto areas = getListOf(GeometryType::Area);
  for(auto rawArea : areas) {
    auto area = dynamic_cast<Area*>(rawArea);
    for(int o=0; o < area->loops.size(); o++) {
      auto loop = area->loops[o];
      for(int i=0; i < loop->lines.size(); i++) {
        auto aLine = area->loops[o]->lines[i].line;
        if (aLine->getID() == line->getID())
          area->loops[o]->lines[i].line = line;
      }
    }
  }
}

void GeometryList::add(Geometry* geometry) {
    if (geometry->getID() == -1)
      throw std::invalid_argument("Somente são permitidos geometrias com ID no GeometryList");

    auto findId = std::find(ids.begin(), ids.end(), geometry->getID());
    while (findId != ids.end()) {
      int pos = std::distance(ids.begin(), findId);
      if (objects[pos]->getGeometryType() == geometry->getGeometryType()) {
        ids[pos] = geometry->getID();
        objects[pos] = geometry;
        if (geometry->getGeometryType() == GeometryType::Line)
          substituteInAreas(dynamic_cast<Line*>(geometry));
        return;
      }
      findId = std::find(std::next(findId), ids.end(), geometry->getID());
    }

    ids.push_back(geometry->getID());
    objects.push_back(geometry);
}

void GeometryList::remove(Geometry* geometry) {
	if (geometry->getID() == -1)
		throw std::invalid_argument("Somente são permitidos geometrias com ID no GeometryList");

	auto findId = std::find(ids.begin(), ids.end(), geometry->getID());
	while (findId != ids.end()) {
		int pos = std::distance(ids.begin(), findId);
		if (objects[pos]->getGeometryType() == geometry->getGeometryType()) {
			ids.erase(findId);
			objects.erase(objects.begin() + pos);
			return;
		}
		findId = std::find(std::next(findId), ids.end(), geometry->getID());
	}
}

std::vector<Geometry*> GeometryList::getListOf(GeometryType geometryType) {
    std::vector<Geometry*> result;
    for(auto i = 0; i < objects.size(); i++) {
        auto obj = objects.at(i);
        if (obj->getGeometryType() == geometryType) {
            result.push_back(obj);
        }
    }
    return result;
}

Geometry* GeometryList::getByID(GeometryType geometryType, int id) {
    auto findId = std::find(ids.begin(), ids.end(), id);
    while (findId != ids.end()) {
      int pos = std::distance(ids.begin(), findId);
      if (objects[pos]->getGeometryType() == geometryType) {
        return objects[pos];
      }
      findId = std::find(std::next(findId), ids.end(), id);
    }
    return NULL;
}

} // geomlib