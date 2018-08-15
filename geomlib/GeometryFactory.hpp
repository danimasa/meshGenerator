#pragma once

#include "Geometry.hpp"
#include "KeyPoint.hpp"
#include "Line.hpp"

namespace geomlib {
    class GeometryFactory {
    public:
        static const GeometryFactory* getDefaultInstance();

        KeyPoint* createKeypoint() const;
        KeyPoint* createKeypoint(Point &point) const;

        Line* createLine(int init_point_id, int final_point_id) const;

        GeometryFactory(GeometryFactory const&) = delete;
        void operator=(GeometryFactory const&) = delete;
    private:
        GeometryFactory() {}
    };
}