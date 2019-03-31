#pragma once

#include "GeometryList.hpp"
#include "Polyline.hpp"
#include "StraightLine.hpp"
#include "ArcLine.hpp"

namespace processlib {

using namespace geomlib;

class LineAnalysis {
public:
    LineAnalysis(GeometryList *geomList)
        : geomList(geomList) {}
    
    void findSingularities();

protected:
    Polyline* brokeInPolyline(Line* line, KeyPoint *pointInLine);
    Polyline* brokeStraightLine(StraightLine* line, KeyPoint *pointInLine);
    Polyline* brokeArcLine(ArcLine* line, KeyPoint *pointInLine);

private:
    GeometryList *geomList;

};

}