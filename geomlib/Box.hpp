#pragma once

#include "Point.hpp"

namespace geomlib {

class Box {
private:
   Point center_point;
   double width;  // d_x
   double height; // d_y
   double depth;  // d_z

public:
    Box(Point center_point,
        double width, double height, double depth) :
        center_point(center_point), width(width),
        height(height), depth(depth) {}


    Point get_center() { return center_point; }
    double get_width() { return width; }
    double get_height() { return height; }
    double get_depth() { return depth; }
};

}