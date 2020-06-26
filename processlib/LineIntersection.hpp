#pragma once

#include <vector>
#include "Line.hpp"
#include "Vector.hpp"

namespace processlib
{

using namespace geomlib;

struct Point_Ints {
    Point intersection;
    std::vector<Line*> segments;
};
 
class LineIntersection {
public:
    struct ContinuousLine {
        KeyPoint* point;
        Vector* direction;
    };

    static std::vector<Point_Ints> findIntersections(std::vector<Line*> &lines);
    static Point findIntersection(const ContinuousLine &l1, const ContinuousLine &l2);

};

} // namespace processlib
