#pragma once

#include <memory>

#include "Vector.hpp"
#include "KeyPoint.hpp"
#include "Geometry.hpp"
#include "Box.hpp"

using namespace std;

namespace geomlib {

    enum class LineType { StraightLine, ArcLine, Polyline, UnspecifiedLine };

    enum class LineDirection {
        DIRECT,
        INVERSE
    };

    class Line : public Geometry {
    public:
        KeyPoint* init_point;
        KeyPoint* final_point;
        
        virtual LineType getLineType() const = 0;
        virtual double length() const = 0;
        virtual Point pointAtPosition(const double position) = 0;
        virtual double isPointInLine(const Point &point) = 0;
        virtual Box outBox() = 0;

        virtual Vector get_init_vector() const = 0;
        virtual Vector get_final_vector() const = 0;

        inline int getElementsQty() { return elementsQty; }
        inline void setElementsQty(int elements) { elementsQty = elements; }

    protected:
        Line(KeyPoint* init_point, KeyPoint* final_point) :
            Geometry(GeometryType::Line),
            init_point(init_point), final_point(final_point) {}

    private:
        int elementsQty;
    };
}

