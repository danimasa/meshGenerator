#pragma once

#include <memory>
#include "Vector.hpp"
#include "KeyPoint.hpp"
#include "Geometry.hpp"

using namespace std;

namespace geomlib {

    class Line : public Geometry {
    public:
        int init_point;
        int final_point;
        double lenght;
        Point* mid_point;
        Vector* tangent_init_vector;
        Vector* tangent_final_vector;
        
        // Construtores somente para friend class
        Line(int init_point, int final_point) :
            init_point(init_point), final_point(final_point) {}
        
        string getGeometryType() const { return "line"; }
    };
}

