#pragma once

#include <string>
#include <memory>

// Forward Declarations
namespace geomlib {
    class GeometryFactory;
}

namespace geomlib {
  
  enum class GeometryType { Keypoint, Line, Area, Volume, Vertex, Element };

  class Geometry {
  public:
    friend class GeometryFactory;

    virtual int getID() const { return ID; }
    virtual void setID(int newID) { ID = newID; }

    GeometryType getGeometryType() { return pvt_type; }

    static int getNextId(GeometryType type);

  protected:
    int ID;

    Geometry(GeometryType type);
    Geometry(const Geometry &geometry);

  private:
    static int next_kp_id;
    static int next_line_id;
    static int next_area_id;
    static int next_volume_id;
    static int next_vertex_id;
    static int next_element_id;

    GeometryType pvt_type;
  };
}