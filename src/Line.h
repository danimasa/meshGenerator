#pragma once

#include "KeyPoint.h"

class Line {
public:
    KeyPoint* init_point;
    KeyPoint* final_point;
    
    Line(KeyPoint* init_point, KeyPoint* final_point) :
        init_point(init_point), final_point(final_point) {}
};
