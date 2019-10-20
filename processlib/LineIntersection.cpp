#include<LineIntersection.hpp>

#include <stdexcept>
#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Sweep_line_2_algorithms.h>

typedef CGAL::Quotient<CGAL::MP_Float>                      NT;
typedef CGAL::Cartesian<NT>                                 Kernel;
typedef Kernel::Point_2                                     Point_2;
typedef CGAL::Arr_segment_traits_2<Kernel>                  Traits_2;
typedef Traits_2::Curve_2                                   Segment_2;

namespace processlib
{

std::list<Segment_2> generateCGALCurveList(vector<Line*> segments) {
    std::list<Segment_2> list;

    for(auto l : segments) {
        if (l->getLineType() != LineType::StraightLine) {
            throw std::invalid_argument("Only Implemented for straight lines");
        }

        Point_2 p1(l->init_point->x, l->init_point->y);
        Point_2 p2(l->final_point->x, l->final_point->y);
        Segment_2 curve(p1, p2);
        list.push_back(curve);
    }

    return list;
}

vector<Point_Ints> LineIntersection::findIntersections() {
    vector<Point_Ints> result;

    std::list<Segment_2> curves = generateCGALCurveList(m_segments);

    std::list<Point_2> pts;
    CGAL::compute_intersection_points(curves.begin(), curves.end(),
        std::back_inserter(pts));

    return result;
}

} // namespace processlib
