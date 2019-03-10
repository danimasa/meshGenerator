#pragma once

#include "Geometry.hpp"
#include "KeyPoint.hpp"
#include "StraightLine.hpp"
#include "UnspecifiedLine.hpp"
#include "ArcLine.hpp"
#include "Polyline.hpp"
#include "Plane.hpp"
#include "Area.hpp"

namespace geomlib {
    class GeometryFactory {
    public:
        static const GeometryFactory* getDefaultInstance();

        KeyPoint* createKeypoint() const;
        KeyPoint* createKeypoint(Point &point) const;

        // TODO: createStraightLine, createArcLine, createGeneralLine
        StraightLine* createStraightLine(KeyPoint* init_point, KeyPoint* final_point) const;
        ArcLine* createArcLine(KeyPoint* init_point, KeyPoint* final_point, Point* mid_point,
            Vector* init_tangent_vector, Vector* final_tangent_vector) const;

        UnspecifiedLine* createUnspecifiedLine(
            KeyPoint* init_point,
            KeyPoint* final_point,
            Point* mid_point,
            Vector* tangent_init_vector,
            Vector* tangent_final_vector,
            double length) const;

        Polyline* createPolyline(
            KeyPoint* init_point,
            KeyPoint* final_point,
            vector<Line*> lines
        ) const;

        Plane* createPlane(Point* p1, Point* p2, Point* p3) const;
        Plane* createPlane(Point* p1, Vector* v1, Vector* v2) const;

        Area* createArea(vector<Line*> lines, Line* first_line, Line* last_line) const;

        GeometryFactory(GeometryFactory const&) = delete;
        void operator=(GeometryFactory const&) = delete;
    private:
        GeometryFactory() {}
    };
}