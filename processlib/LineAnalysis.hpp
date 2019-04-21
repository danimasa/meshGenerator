#pragma once
#include <map>

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
    void brokeAndSubstitute(Line *line, Line *innerLine, KeyPoint *brokePoint);

private:
    GeometryList *geomList;
    map<int, int> processedLinePoint;

	vector<Line*> substituteCommomLine(vector<Line*> newLines, Polyline* polyline);
};

}