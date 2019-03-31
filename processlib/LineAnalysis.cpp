#include <vector>

#include "LineAnalysis.hpp"
#include "GeometryFactory.hpp"

namespace processlib
{

using namespace geomlib;

Polyline* LineAnalysis::brokeStraightLine(StraightLine *line, KeyPoint *pointInLine) {
  auto factory = GeometryFactory::getDefaultInstance();

  auto line1 = factory->createStraightLine(line->init_point, pointInLine);
  auto line2 = factory->createStraightLine(pointInLine, line->final_point);

  geomList->add(line1);
  geomList->add(line2);

  vector<Line*> lines {line1, line2};
  auto polyline = factory->createPolyline(line->init_point, line->final_point, lines);
  return polyline;
}

Polyline* LineAnalysis::brokeArcLine(ArcLine *line, KeyPoint *pointInLine) {
  auto factory = GeometryFactory::getDefaultInstance();

  auto line1 = factory->createArcLine(line->init_point, pointInLine, line->get_plane(),
    line->get_radius(), line->get_center());
  auto line2 = factory->createArcLine(pointInLine, line->final_point, line->get_plane(),
    line->get_radius(), line->get_center());

  geomList->add(line1);
  geomList->add(line2);

  vector<Line*> lines {line1, line2};
  auto polyline = factory->createPolyline(line->init_point, line->final_point, lines);
  return polyline;
}

Polyline* LineAnalysis::brokeInPolyline(Line *line, KeyPoint *pointInLine) {
  switch (line->getLineType())
  {
    case LineType::StraightLine:
      return brokeStraightLine(dynamic_cast<StraightLine*>(line), pointInLine);
    case LineType::ArcLine:
      return brokeArcLine(dynamic_cast<ArcLine*>(line), pointInLine);
    // case LineType::Polyline:
    default:
      return nullptr;
  }
}

void LineAnalysis::findSingularities()
{
  auto lines = geomList->getListOf(GeometryType::Line);
  for (auto l : lines)
  {
    auto line = dynamic_cast<Line *>(l);
    for (auto i : lines)
    {
      auto innerLine = dynamic_cast<Line *>(i);
      if (innerLine->getID() != line->getID() && innerLine->getLineType() != LineType::UnspecifiedLine)
      {
        // Caso 1
        double initPosition = innerLine->isPointInLine(*line->init_point);
        double finalPosition = innerLine->isPointInLine(*line->final_point);
        if (initPosition > 0 && initPosition < 1)
        {
          auto polyline = brokeInPolyline(innerLine, line->init_point);
          polyline->setID(innerLine->getID());
          geomList->add(polyline);
        }
        if (finalPosition > 0 && finalPosition < 1)
        {
          auto polyline = brokeInPolyline(innerLine, line->final_point);
          polyline->setID(innerLine->getID());
          geomList->add(polyline);
        }
      }
    }
  }
}

} // namespace processlib