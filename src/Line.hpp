#pragma once

#include <memory>
#include "KeyPoint.hpp"

using namespace std;

struct Line {
    shared_ptr<KeyPoint> init_point;
    shared_ptr<KeyPoint> final_point;
    
    Line(shared_ptr<KeyPoint> init_point, shared_ptr<KeyPoint> final_point) :
        init_point(init_point), final_point(final_point) {}
};
