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
        : m_segments(lines) {}

    vector<Point_Ints> findIntersections();

private:
    vector<Line*> m_segments;
};

} // namespace processlib
