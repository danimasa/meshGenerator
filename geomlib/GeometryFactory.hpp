#pragma once

#include "Geometry.hpp"
#include "KeyPoint.hpp"
#include "StraightLine.hpp"
#include "ArcLine.hpp"

namespace geomlib {
    class GeometryFactory {
    public:
        static const GeometryFactory* getDefaultInstance();

        KeyPoint* createKeypoint() const;
        KeyPoint* createKeypoint(Point &point) const;

        // TODO: createStraightLine, createArcLine, createGeneralLine
        StraightLine* createStraightLine(KeyPoint* init_point, KeyPoint* final_point) const;

        GeometryFactory(GeometryFactory const&) = delete;
        void operator=(GeometryFactory const&) = delete;
    private:
        GeometryFactory() {}
    };
}