#pragma once

#include <vector>
#include <string>
#include "Geometry.hpp"

namespace geomlib {

    class GeometryList {
    public:
        GeometryList() {}

        void add(Geometry* geometry);

        void reserveSize(int legth);
        int capacity() const;
        int size() const;

        std::vector<Geometry*> getListOf(GeometryType geometryType);
        Geometry* getByID(GeometryType geometryType, int id);

    private:
        std::vector<Geometry*> objects;
        std::vector<int> ids;
    };

}