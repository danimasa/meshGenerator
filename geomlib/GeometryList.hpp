#pragma once

#include <vector>
#include <string>
#include "Geometry.hpp"

namespace geomlib {

    class GeometryList {
    public:
        GeometryList() : objects(std::vector<Geometry*>()) {}

        void add(Geometry* geometry);

        void reserveSize(int legth);
        int capacity() const;
        int size() const;

        std::vector<Geometry*> getListOf(std::string geometryType);

    private:
        std::vector<Geometry*> objects;
    };

}