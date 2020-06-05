#pragma once

#include <memory>
#include <vector>

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

    class Area;
    class Polyline;

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

        virtual vector<Area*> getAttachedAreas() const = 0;

    protected:
        Line(KeyPoint* init_point, KeyPoint* final_point) :
            Geometry(GeometryType::Line),
            init_point(init_point), final_point(final_point) {}

        vector<Area*> attachedAreas;

    private:
        int elementsQty;

        friend class Area;
        friend class Polyline;
    };
}

