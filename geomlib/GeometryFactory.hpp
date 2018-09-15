#pragma once

#include "Geometry.hpp"
#include "KeyPoint.hpp"
#include "StraightLine.hpp"
#include "UnspecifiedLine.hpp"
#include "ArcLine.hpp"
#include "Plane.hpp"

namespace geomlib {
    class GeometryFactory {
    public:
        static const GeometryFactory* getDefaultInstance();

        KeyPoint* createKeypoint() const;
        KeyPoint* createKeypoint(Point &point) const;

        // TODO: createStraightLine, createArcLine, createGeneralLine
        StraightLine* createStraightLine(KeyPoint* init_point, KeyPoint* final_point) const;
        UnspecifiedLine* createUnspecifiedLine(
            KeyPoint* init_point,
            KeyPoint* final_point,
            Point* mid_point,
            Vector* tangent_init_vector,
            Vector* tangent_final_vector) const;

        Plane* createPlane(Point* p1, Point* p2, Point* p3) const;
        Plane* createPlane(Point* p1, Vector* v1, Vector* v2) const;

        GeometryFactory(GeometryFactory const&) = delete;
        void operator=(GeometryFactory const&) = delete;
    private:
        GeometryFactory() {}
    };
}