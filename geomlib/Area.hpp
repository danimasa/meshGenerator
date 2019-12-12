#pragma once

#include <vector>

#include "Geometry.hpp"
#include "Line.hpp"

namespace geomlib
{

class GeometryFactory;

class Area : public Geometry {
public:
  struct LoopLine {
    Line* line;
    LineDirection direction;
  };

  struct Loop {
    vector<LoopLine> lines;
    
    vector<Line*> getLines() const;

    Loop(vector<Line*> lines);
  };

  vector<Loop*> loops;

  GeometryType getGeometryType() const { return GeometryType::Area; }

private:
  Area(vector<Loop*> loops) :
    Geometry(GeometryType::Area),
     loops(loops) {}

  friend class GeometryFactory;
}; // Area

} // geomlib
