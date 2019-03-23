#pragma once

#include <memory>
#include "Vector.hpp"
#include "KeyPoint.hpp"
#include "Geometry.hpp"

using namespace std;

namespace geomlib {

    enum class LineType { StraightLine, ArcLine, Polyline, UnspecifiedLine };

    class Line : public Geometry {
    public:
        KeyPoint* init_point;
        KeyPoint* final_point;
        
        virtual LineType getLineType() const = 0;
        virtual double length() const = 0;
        virtual Point pointInLine(const double position) = 0;

    protected:
        Line(KeyPoint* init_point, KeyPoint* final_point) :
            Geometry(GeometryType::Line),
            init_point(init_point), final_point(final_point) {}
    };
}

