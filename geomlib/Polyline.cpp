#include "Polyline.hpp"

using namespace std;

namespace geomlib {

Polyline::Line_in_Polyline nextTouchingLine(const Point& point, vector<Line*>& remainingLines) {
  int index = -1;
  Line* searchedLine = NULL;
  for(int i = 0; i < remainingLines.size(); i++) {
    auto l = remainingLines[i];
    if(*l->init_point == point || *l->final_point == point) {
      if (searchedLine == NULL) {
        index = i;
        searchedLine = l;
      }
      else throw std::invalid_argument("Polyline continuity problem, more than one line sharing the same point");
    }
  }

  if (searchedLine == NULL)
    throw std::invalid_argument("Polyline problem: not touching lines finded");

  remainingLines.erase(remainingLines.begin() + index);
  
  Polyline::LINE_DIRECTION lineDirection = Polyline::LINE_DIRECTION::DIRECT;
  if (*searchedLine->final_point == point) lineDirection = Polyline::LINE_DIRECTION::INVERSE;
  return Polyline::Line_in_Polyline{ searchedLine, lineDirection };
}

Polyline::Polyline(
  KeyPoint* init_point,
  KeyPoint* final_point,
  vector<Line*> lineList
) : Line(init_point, final_point)
{
  if(init_point == final_point)
    throw std::invalid_argument("It's is a loop not a polyline (equal init and final points)");

  if(lineList.size() < 2)
    throw std::invalid_argument("A polyline must have at least two lines");

  vector<Line*> remainingLines = lineList;
  Point searchedPoint = *init_point;
  Point finalPoint = *final_point;
  do {
    Line_in_Polyline searchedLine = nextTouchingLine(searchedPoint, remainingLines);
    lines.push_back(searchedLine);

    if(searchedLine.direction == LINE_DIRECTION::DIRECT) searchedPoint = *searchedLine.line->final_point;
    else searchedPoint = *searchedLine.line->init_point;
  } while(searchedPoint != finalPoint);
}

double Polyline::length() const {
  double sum_of_lenghts = 0.0;
  for(auto& n : lines)
    sum_of_lenghts += n.line->length();
  return sum_of_lenghts;
}

Point Polyline::pointInLine(const double position) {
    if (position < 0 || position > 1)
        throw std::invalid_argument("pointInLine so aceita valores entre 0 e 1");

    if (position == 0) return *init_point;
    if (position == 1) return *final_point;

    double totalLength = length();
    double maxLinePosition = 0;
    double sumPassedPosition = 0;
    auto litr = lines.begin();
    maxLinePosition = litr->line->length() / totalLength;
    while(maxLinePosition < position) {
      sumPassedPosition += maxLinePosition;
      litr++;
      maxLinePosition = sumPassedPosition + ( litr->line->length() / totalLength );
    } 

    Line_in_Polyline lineWIthPoint = *litr;
    double linePosition = position - sumPassedPosition;
    return lineWIthPoint.line->pointInLine(linePosition);
}

vector<Line*> Polyline::get_lines() const {
  vector<Line*> lineList(lines.size());
  for(auto& l : lines)
    lineList.push_back(l.line);
  return lineList;
}

}