#include <vector>

#include "LineAnalysis.hpp"
#include "GeometryFactory.hpp"
#include "mathUtils.hpp"

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

vector<Line*> substituteCommomLine(vector<Line*> newLines, Polyline* polyline) {
  for(int i=0; i < newLines.size(); i++) {
    auto l = newLines[i];
    for(auto pl : polyline->get_lines()) {
      if((l->init_point == pl->init_point || l->init_point == pl->final_point)
         && (l->final_point == pl->final_point || l->final_point == pl->init_point)
         && l->getLineType() == pl->getLineType()
      ) {
        newLines[i] = pl;
        return newLines;
      }
    }
  }
  return newLines;
}

void LineAnalysis::brokeAndSubstitute(Line *line, Line *innerLine, KeyPoint *brokePoint) {
  auto polyline = brokeInPolyline(innerLine, brokePoint);
  polyline->setID(innerLine->getID());
  geomList->add(polyline);

  Vector vLine1 (line->init_point, line->final_point);
  Vector vLine2 (innerLine->init_point, innerLine->final_point);
  double angle = vLine1.angleWith(vLine2);
  if (double_equals(angle, 0) || double_equals(angle, M_PI)) {
    // Caso 2 
    auto factory = GeometryFactory::getDefaultInstance();

    double pointInLine = line->isPointInLine(*innerLine->init_point);
    KeyPoint *secBrokePoint = pointInLine > 0 && pointInLine < 1
      ? innerLine->init_point
      : innerLine->final_point;

    auto refPolyline = brokeInPolyline(line, secBrokePoint);
    auto newLines = substituteCommomLine(refPolyline->get_lines(), polyline);
    auto secPolyline = factory->createPolyline(line->init_point, line->final_point, newLines);
    secPolyline->setID(line->getID());
    geomList->add(secPolyline);
    processedLinePoint[line->getID()] = secBrokePoint->getID();
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
        if (initPosition > 0 && initPosition < 1
          && processedLinePoint[innerLine->getID()] != line->init_point->getID())
          brokeAndSubstitute(line, innerLine, line->init_point);
        if (finalPosition > 0 && finalPosition < 1
          && processedLinePoint[innerLine->getID()] != line->final_point->getID())
          brokeAndSubstitute(line, innerLine, line->final_point);
     }
    }
  }
}

} // namespace processlib