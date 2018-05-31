#pragma once

#include <memory>
#include "KeyPoint.hpp"
#include "Identifiable.hpp"

using namespace std;

class Line : public Identifiable {
public:
    shared_ptr<KeyPoint> init_point;
    shared_ptr<KeyPoint> final_point;
    
    Line(shared_ptr<KeyPoint> init_point, shared_ptr<KeyPoint> final_point) :
        init_point(init_point), final_point(final_point) {}
};
