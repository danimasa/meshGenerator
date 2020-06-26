#pragma once

#include <vector>

#include "Geometry.hpp"
#include "Line.hpp"

namespace geomlib
{

class GeometryFactory;

class Area : public Geometry {
public:
  struct Loop {
    vector<Line*> lines;

    Loop(vector<Line*> lines)
      : lines(lines) {}
  };

  vector<Loop*> loops;

  GeometryType getGeometryType() const { return GeometryType::Area; }

protected:
  vector<Area*> children;

private:
  Area(vector<Loop*> loops);
  ~Area();

  friend class GeometryFactory;
}; // Area

} // geomlib
