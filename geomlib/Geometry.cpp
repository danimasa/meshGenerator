#include "Geometry.hpp"
#include "GeometryFactory.hpp"

namespace geomlib {

int Geometry::next_kp_id = 1;
int Geometry::next_line_id = 1;
int Geometry::next_area_id = 1;
int Geometry::next_volume_id = 1;
int Geometry::next_vertex_id = 1;
int Geometry::next_element_id = 1;

int Geometry::getNextId(GeometryType type) {
    switch (type)
    {
        case GeometryType::Keypoint:
            return next_kp_id++;
        case GeometryType::Line:
            return next_line_id++;
        case GeometryType::Area:
            return next_area_id++;
        case GeometryType::Volume:
            return next_volume_id++;
        case GeometryType::Vertex:
            return next_vertex_id++;
        case GeometryType::Element:
            return next_element_id++;
    }
}

Geometry::Geometry(GeometryType type)
{
    ID = getNextId(type);
    pvt_type = type;
}

Geometry::Geometry(const Geometry &geometry)
    : ID(geometry.ID), pvt_type(geometry.pvt_type)
{
}

}