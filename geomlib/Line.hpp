#pragma once

#include <memory>
#include "Vector.hpp"
#include "KeyPoint.hpp"
#include "Geometry.hpp"

using namespace std;

namespace geomlib {

    class Line : public Geometry {
    public:
        KeyPoint* init_point;
        KeyPoint* final_point;
        
        string getGeometryType() const { return "line"; }

        virtual string getLineType() const = 0;
        virtual double length() const = 0;
        virtual Point pointInLine(const double position) = 0;

    protected:
        Line(KeyPoint* init_point, KeyPoint* final_point) :
            init_point(init_point), final_point(final_point) {}
    };
}

