#include<LineIntersection.hpp>

#include <stdexcept>
#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Sweep_line_2_algorithms.h>
#include <CGAL/box_intersection_d.h>
#include <CGAL/intersections.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2                                     Point_2;
typedef Kernel::Point_3                                     Point_3;
typedef Kernel::Vector_3                                    Vector_3;
typedef CGAL::Arr_segment_traits_2<Kernel>                  Traits_2;
typedef Traits_2::Curve_2                                   Segment_2;
typedef Kernel::Segment_3                                   Segment_3;
typedef Kernel::Intersect_3                                 Intersect_3;
typedef Kernel::Line_3                                      Line_3;

namespace processlib
{

struct Segment_Cgal {
    Line* line;
    Segment_3 segment;
};

typedef std::vector<Segment_Cgal>                           Segments;
typedef Segments::iterator                                  Iterator;
typedef CGAL::Box_intersection_d::Box_with_handle_d<double,3,Iterator> CgalBox;

Segments generateCGALCurveList(vector<Line*> segments) {
    Segments list;

    for(auto l : segments) {
        if (l->getLineType() != LineType::StraightLine) {
            throw std::invalid_argument("Only Implemented for straight lines");
        }

        Point_3 p(l->init_point->x, l->init_point->y, l->init_point->z);
        Point_3 q(l->final_point->x, l->final_point->y, l->final_point->z);
        Segment_3 curve(p, q);
        Segment_Cgal seg {l, curve};
        list.push_back(seg);
    }

    return list;
}

vector<Point_Ints> LineIntersection::findIntersections(vector<Line*> &lines) {
    Segments curves = generateCGALCurveList(lines);
    vector<Point_Ints> m_intersections;

    std::vector<CgalBox> boxes;
    for (Iterator i = curves.begin(); i != curves.end(); ++i)
        boxes.push_back(CgalBox(i->segment.bbox(), i));

    std::vector<CgalBox *> ptr;
    for (std::vector<CgalBox>::iterator i = boxes.begin(); i != boxes.end(); ++i)
        ptr.push_back(&*i);

    CGAL::box_self_intersection_d(ptr.begin(), ptr.end(), [&](const CgalBox *a, const CgalBox *b){
       if(!a->handle()->segment.is_degenerate() && !b->handle()->segment.is_degenerate() &&
            CGAL::do_intersect(a->handle()->segment, b->handle()->segment)) {
        
            auto cgal_line_1 = a->handle()->segment;
            auto cgal_line_2 = b->handle()->segment;
            auto line_1 = a->handle()->line;
            auto line_2 = b->handle()->line;
            Point intersection_point;
            CGAL::cpp11::result_of<Intersect_3(Segment_3, Segment_3)>::type
                r = CGAL::intersection(cgal_line_1, cgal_line_2);
            if (r) {
                if(const Point_3* p = boost::get<Point_3>(&*r)) {
                    intersection_point.x = p->x();
                    intersection_point.y = p->y();
                    intersection_point.z = p->z();
                } else {
                    return;
                }
            }

            if(intersection_point != *(line_1->final_point)
                && intersection_point != *(line_1->init_point)
                && intersection_point != *(line_2->final_point)
                && intersection_point != *(line_2->init_point)) {

                vector<Line*> lines {line_1, line_2};
                Point_Ints intersection {intersection_point, lines};
                m_intersections.push_back(intersection);
            }
       }
    });

    return m_intersections;
}

Point LineIntersection::findIntersection(const ContinuousLine &l1, const ContinuousLine &l2) {
    Point_3 p1(l1.point->x, l1.point->y, l1.point->z);
    Vector_3 v1(l1.direction->x, l1.direction->y, l1.direction->z);
    Line_3 line1 (p1, v1);

    Point_3 p2(l2.point->x, l2.point->y, l2.point->z);
    Vector_3 v2(l2.direction->x, l2.direction->y, l2.direction->z);
    Line_3 line2 (p2, v2);

    auto result = CGAL::intersection(line1, line2);
    if (result) {
        if (const Point_3* s = boost::get<Point_3>(&*result)) {
            return Point(s->x(), s->y(), s->z());
        }
    }

    throw "Lines Intersections not found.";
}

} // namespace processlib
