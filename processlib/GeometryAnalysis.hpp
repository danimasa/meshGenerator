#pragma once
#include <map>

#include "GeometryList.hpp"
#include "Polyline.hpp"
#include "StraightLine.hpp"
#include "ArcLine.hpp"

namespace processlib {

using namespace geomlib;

class GeometryAnalysis {
protected:
    GeometryAnalysis(GeometryList *geomList)
        : geomList(geomList) {}

    Polyline* brokeInPolyline(Line* line, KeyPoint *pointInLine);
    Polyline* brokeStraightLine(StraightLine* line, KeyPoint *pointInLine);
    Polyline* brokeArcLine(ArcLine* line, KeyPoint *pointInLine);
    void brokeAndSubstitute(Line *line, Line *innerLine, KeyPoint *brokePoint);

	vector<Line*> substituteCommomLine(vector<Line*> newLines, Polyline* polyline);

    GeometryList *geomList;
    map<int, int> processedLinePoint;
};

}