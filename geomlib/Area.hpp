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
  vector<Area*> getChildren() const { return children; }
  void addChild(Area* area);
  
  GeometryType getGeometryType() const { return GeometryType::Area; }

private:
  Area(vector<Loop*> loops);
  ~Area();

  vector<Area*> children;

  friend class GeometryFactory;
}; // Area

} // geomlib
