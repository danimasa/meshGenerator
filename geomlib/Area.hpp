#pragma once

#include <vector>

#include "Geometry.hpp"
#include "Line.hpp"

namespace geomlib
{

class GeometryFactory;

class Area : public Geometry {
public:
  vector<Line*> lines;
  Line* first_line;
  Line* last_line;

  GeometryType getGeometryType() const { return GeometryType::Area; }

private:
  Area(vector<Line*> lines, Line* first_line, Line* last_line) :
    Geometry(GeometryType::Area),
    lines(lines), first_line(first_line), last_line(last_line) {}

  friend class GeometryFactory;
}; // Area

} // geomlib
