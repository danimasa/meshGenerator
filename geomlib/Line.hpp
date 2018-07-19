#pragma once

#include <memory>
#include "Vector.hpp"
#include "KeyPoint.hpp"
#include "Identifiable.hpp"

using namespace std;

class Line : public Identifiable {
public:
    shared_ptr<KeyPoint> init_point;
    shared_ptr<KeyPoint> final_point;
    double lenght;
    unique_ptr<Point> mid_point;
    unique_ptr<Vector> tangent_init_vector;
    unique_ptr<Vector> tangent_final_vector;
    
    Line(shared_ptr<KeyPoint> init_point, shared_ptr<KeyPoint> final_point) :
        init_point(init_point), final_point(final_point) {}
    
    Line(shared_ptr<KeyPoint> init_point, shared_ptr<KeyPoint> final_point, 
        const double lenght, Point* mid_point, 
        Vector* tangent_init_vector, Vector* tangent_final_vector) :
        init_point(init_point), final_point(final_point), lenght(lenght),
        mid_point(mid_point), tangent_init_vector(tangent_init_vector),
        tangent_final_vector(tangent_final_vector) {}
};
