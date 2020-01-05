#pragma once

#include <vector>
#include "Line.hpp"

namespace geomlib
{

using namespace std;

// Final quadrilateral poligon for mesh generation
class QuadArea {
public:
  struct TopologicalLine {
    Line* line;
    LineDirection direction;
  };

  QuadArea(vector<Line*> lines);
  vector<Line*> getLines() const;

  vector<TopologicalLine> lines;
};

} // namespace geomlib
