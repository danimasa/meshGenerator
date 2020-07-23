#pragma once

#include "Line.hpp"
#include "Point.hpp"
#include <vector>

namespace meshlib
{

using namespace std;
using namespace geomlib;

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
  vector<KeyPoint*> getVertex() const;

  TopologicalLine south() const { return lines[0]; }
  TopologicalLine east() const { return lines[1]; }
  TopologicalLine north() const { return lines[2]; }
  TopologicalLine west() const { return lines[3]; }

  bool someQtdElementsZero() const;
  int sumQtdElements() const;
  bool isEvenElements() const;
  // void determineLinesSubdivision(double elementSize);

  vector<TopologicalLine> lines;
  double attenuationAngleRatio = 0.5;
};

} // namespace geomlib
