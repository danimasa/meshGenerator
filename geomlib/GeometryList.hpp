#pragma once

#include <vector>
#include <string>
#include "Geometry.hpp"

namespace geomlib {

    class GeometryList {
    public:
        void add(Geometry* geometry);
        void remove(Geometry* geometry);

        void reserveSize(int legth);

        std::vector<Geometry*> getListOf(std::string geometryType);

    private:
        std::vector<Geometry*> objects;
    };

}