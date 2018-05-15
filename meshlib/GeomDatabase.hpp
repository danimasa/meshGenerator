#pragma once

#include <vector>
#include "KeyPoint.hpp"
#include "Line.hpp"

using namespace std;

class GeomDatabase {
public:
    vector<KeyPoint> keyPoints;
    vector<Line> lines;
};