#pragma once

#include <vector>
#include "LineMesh.hpp"

namespace meshlib
{

using namespace std;
using namespace geomlib;

// Final quadrilateral poligon for mesh generation
class QuadArea {
public:
  struct TopologicalLine {
    LineMesh* line;
    LineDirection direction;
    int qtdElements;
  };

  QuadArea(vector<LineMesh*> lines);
  QuadArea(vector<LineMesh*> lines, double attenuationAngleRatio);
  vector<LineMesh*> getLines() const;
  vector<Vertex*> getVertex() const;

  TopologicalLine south() const { return lines[0]; }
  TopologicalLine east() const { return lines[1]; }
  TopologicalLine north() const { return lines[2]; }
  TopologicalLine west() const { return lines[3]; }

  bool someQtdElementsZero() const;
  int sumQtdElements() const;
  bool isEvenElements() const;
  void determineLinesSubdivision(double elementSize);

  vector<TopologicalLine> lines;
  double attenuationAngleRatio = 0.5;
};

} // namespace geomlib
