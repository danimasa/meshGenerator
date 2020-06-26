#pragma once

#include "GeometryAnalysis.hpp"

namespace processlib {

using namespace geomlib;

class LineAnalysis : public GeometryAnalysis {
public:
    LineAnalysis(GeometryList *geomList)
        : GeometryAnalysis(geomList) {}
    
    void findSingularities();
};

}