#pragma once

#include <vector>
#include <Line.hpp>

namespace processlib
{

using namespace geomlib;

struct Point_Ints {
    Point intersection;
    vector<Line*> segments;
};
 
class LineIntersection {
public:
    LineIntersection(vector<Line*> &lines)
        : m_segments(lines),
        m_intersections(vector<Point_Ints>()) {}

    vector<Point_Ints> findIntersections();

private:
    vector<Line*> m_segments;
    vector<Point_Ints> m_intersections;
};

} // namespace processlib
