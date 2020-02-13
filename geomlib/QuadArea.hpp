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
    int qtdElements;
  };

  QuadArea(vector<Line*> lines);
  vector<Line*> getLines() const;
  vector<Point*> getVertex() const;

  TopologicalLine south() { return lines[0]; }
  TopologicalLine east() { return lines[1]; }
  TopologicalLine north() { return lines[2]; }
  TopologicalLine west() { return lines[3]; }

  bool someQtdElementsZero();
  int sumQtdElements();

  vector<TopologicalLine> lines;
};

} // namespace geomlib
