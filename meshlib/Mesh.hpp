#pragma once

#include <vector>
#include "KeyPoint.hpp"
#include "Line.hpp"

using namespace std;

class Mesh {
   vector<KeyPoint> points;
   vector<Line> lines; 
};