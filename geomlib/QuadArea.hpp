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
  QuadArea(vector<Line*> lines, double attenuationAngleRatio);
  vector<Line*> getLines() const;
  vector<Point*> getVertex() const;

  TopologicalLine south() const { return lines[0]; }
  TopologicalLine east() const { return lines[1]; }
  TopologicalLine north() const { return lines[2]; }
  TopologicalLine west() const { return lines[3]; }

  bool someQtdElementsZero() const;
  int sumQtdElements() const;
  bool isEvenElements() const;

  vector<TopologicalLine> lines;
  double attenuationAngleRatio = 0.5;
};

} // namespace geomlib
