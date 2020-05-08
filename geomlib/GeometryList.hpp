#pragma once

#include <vector>
#include <string>
#include "Geometry.hpp"
#include "Line.hpp"

namespace geomlib {

    class GeometryList {
    public:
        GeometryList() {}

        void add(Geometry* geometry);
		void remove(Geometry* geometry);

        void reserveSize(int legth);
        int capacity() const;
        int size() const;

        Line* findOrGenerateStraightLine(KeyPoint* init_point, KeyPoint* final_point);

        std::vector<Geometry*> getListOf(GeometryType geometryType) const;
        Geometry* getByID(GeometryType geometryType, int id) const;

    private:
        std::vector<Geometry*> objects;
        std::vector<int> ids;

        void substituteInAreas(Line *line);
    };

}