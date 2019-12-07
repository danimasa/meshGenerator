#pragma once

#include <vector>

#include "Geometry.hpp"
#include "Line.hpp"

namespace geomlib
{

class GeometryFactory;

class Area : public Geometry {
public:
  class Loop {
    public:
    vector<Line*> lines;

    Loop(vector<Line*> lines) : lines(lines) {}
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
