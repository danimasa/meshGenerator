#define _USE_MATH_DEFINES
#include <vector>
#include <algorithm>

#include "LineAnalysis.hpp"
#include "GeometryFactory.hpp"
#include "mathUtils.hpp"
#include "LineIntersection.hpp"

namespace processlib
{

using namespace geomlib;

void LineAnalysis::findSingularities()
{
  auto lines = geomList->getListOf(GeometryType::Line);
  std::vector<Line*> lvector;
  auto factory = GeometryFactory::getDefaultInstance();
  for (auto l : lines)
  {
    auto line = dynamic_cast<Line *>(l);
    if (line->getLineType() != LineType::StraightLine) continue;
    lvector.push_back(line);
    for (auto i : lines)
    {
      auto innerLine = dynamic_cast<Line *>(i);
      if (innerLine->getID() != line->getID() && innerLine->getLineType() == LineType::StraightLine)
      {
        // Caso 1
        double initPosition = innerLine->isPointInLine(*line->init_point);
        double finalPosition = innerLine->isPointInLine(*line->final_point);

        bool initInLine = initPosition > 0 && initPosition < 1;
        bool finalInLine = finalPosition > 0 && finalPosition < 1;

        if (initInLine && finalInLine) { // Casos 7 e 8
          auto innerInitPoint = initPosition < finalPosition
            ? line->init_point : line->final_point;
          auto innerFinalPoint = initPosition < finalPosition
            ? line->final_point : line->init_point;

          // auto l1 = factory->createStraightLine(innerLine->init_point, innerInitPoint);
          auto l1 = geomList->findOrGenerateStraightLine(innerLine->init_point, innerInitPoint);
          auto l2 = geomList->findOrGenerateStraightLine(innerFinalPoint, innerLine->final_point);
          vector<Line*> lines { l1, line, l2 };
          auto polyline = factory->createPolyline(innerLine->init_point, innerLine->final_point, lines);
          polyline->setAttachedAreas(innerLine->getAttachedAreas());
          polyline->setID(innerLine->getID());
          geomList->add(polyline);
        }
        else if ((initInLine || finalInLine)
          && (line->final_point == innerLine->init_point
          || line->final_point == innerLine->final_point
          || line->init_point == innerLine->init_point
          || line->init_point == innerLine->final_point)) { // Casos 9 e 10
            KeyPoint *innerInitPoint = nullptr;
            KeyPoint *innerFinalPoint = nullptr;
            if (initInLine) {
              innerInitPoint = line->final_point == innerLine->init_point
                ? line->init_point : innerLine->init_point;
              innerFinalPoint = line->final_point == innerLine->init_point
                ? innerLine->final_point : line->init_point;
            } else {
              innerInitPoint = line->init_point == innerLine->final_point
                ? innerLine->init_point : line->final_point;
              innerFinalPoint = line->init_point == innerLine->final_point
                ? line->final_point : innerLine->final_point;
            }

            auto l1 = geomList->findOrGenerateStraightLine(innerInitPoint, innerFinalPoint);
            vector<Line*> lines { line, l1 };
            auto polyline = factory->createPolyline(innerLine->init_point, innerLine->final_point, lines);
            polyline->setAttachedAreas(innerLine->getAttachedAreas());
            polyline->setID(innerLine->getID());
            geomList->add(polyline);
        }
        else if (initInLine && processedLinePoint[innerLine->getID()] != line->init_point->getID())
          brokeAndSubstitute(line, innerLine, line->init_point);
        else if (finalInLine && processedLinePoint[innerLine->getID()] != line->final_point->getID())
          brokeAndSubstitute(line, innerLine, line->final_point);
        }
     }
    }

    auto intersections = LineIntersection::findIntersections(lvector);

    for(auto inters : intersections) {
      auto kpInter = factory->createKeypoint(inters.intersection);

      auto polyline1 = brokeInPolyline(inters.segments[0], kpInter);
      polyline1->setID(inters.segments[0]->getID());
      geomList->add(polyline1);

      auto polyline2 = brokeInPolyline(inters.segments[1], kpInter);
      polyline2->setID(inters.segments[1]->getID());
      geomList->add(polyline2);
    }
}

} // namespace processlib