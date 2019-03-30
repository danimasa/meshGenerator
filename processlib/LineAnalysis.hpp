#pragma once

#include "GeometryList.hpp"

namespace processlib {

using namespace geomlib;

class LineAnalysis {
public:
    LineAnalysis(GeometryList *geomList)
        : geomList(geomList) {}
    
    void findSingularities();

private:
    GeometryList *geomList;

};

}