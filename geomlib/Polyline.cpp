#include <stdexcept>
#include "Polyline.hpp"

using namespace std;

namespace geomlib {

bool pointInOnlyOneLine(const KeyPoint& point, vector<Line*>& lines) {
  bool firstLineFound = false;
  for(int i = 0; i < lines.size(); i++) {
    auto l = lines[i];
    if(*l->init_point == point || *l->final_point == point) {
      if (!firstLineFound) {
        firstLineFound = true;
      }
      else return false;
    }
  }
  return true;
}

Polyline::Line_in_Polyline nextTouchingLine(const KeyPoint& point, vector<Line*>& remainingLines) {
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
  
  LineDirection lineDirection = LineDirection::DIRECT;
  if (*searchedLine->final_point == point) lineDirection = LineDirection::INVERSE;
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
  KeyPoint searchedPoint = *init_point;
  KeyPoint finalPoint = *final_point;

  if(!pointInOnlyOneLine(searchedPoint, remainingLines)) {
    searchedPoint = *final_point;
    finalPoint = *init_point;
  }

  do {
    Line_in_Polyline searchedLine = nextTouchingLine(searchedPoint, remainingLines);
    lines.push_back(searchedLine);

    if(searchedLine.direction == LineDirection::DIRECT) searchedPoint = *searchedLine.line->final_point;
    else searchedPoint = *searchedLine.line->init_point;
  } while(searchedPoint != finalPoint);

  int linesSize = lines.size(); // if 1 the init and final point is in the same line
  Line* init_line = lines[0].line;
  Line* final_line = lines[lines.size() - 1].line;

  if(linesSize == 1 || !(init_line->init_point == init_point || init_line->final_point == init_point) ||
     !(final_line->init_point == final_point || final_line->final_point == final_point)) {
       throw std::invalid_argument("Polyline continuity problem, could not possible to start and finish in the provided points");
  }
}

double Polyline::length() const {
  double sum_of_lenghts = 0.0;
  for(auto& n : lines)
    sum_of_lenghts += n.line->length();
  return sum_of_lenghts;
}

double Polyline::isPointInLine(const Point &point) {
  if( point == *init_point ) return 0;
  if( point == *final_point ) return 1;

  double globalPosition = 0;
  double l = length();
  for(auto &n : lines) {
    double position = n.line->isPointInLine(point);
    if(position != -1)
      return globalPosition + (position * n.line->length() / l);
    globalPosition += n.line->length() / l;
  }

  return -1;
}

Point Polyline::pointAtPosition(const double position) {
    if (position < 0 || position > 1)
        throw std::invalid_argument("pointInLine so aceita valores entre 0 e 1");

    if (position == 0) return *init_point;
    if (position == 1) return *final_point;

    double totalLength = length();
    double maxLinePosition = 0;
    double sumPassedPosition = 0;
    double initialLinePosition = 0;
    double finalLinePosition = 0;
    auto litr = lines.begin();
    double cPos = litr->line->length() / totalLength;
    maxLinePosition = cPos;
    finalLinePosition = maxLinePosition;
    while(maxLinePosition < position) {
      initialLinePosition = maxLinePosition;
      litr++;
      cPos = litr->line->length() / totalLength;
      maxLinePosition += cPos;
      finalLinePosition = maxLinePosition;
    } 

    Line_in_Polyline lineWIthPoint = *litr;
    double linePosition = (position - initialLinePosition) / (finalLinePosition - initialLinePosition);
    if (lineWIthPoint.direction == LineDirection::INVERSE) {
      linePosition = 1 - linePosition;
    }
    auto point = lineWIthPoint.line->pointAtPosition(linePosition); 
    return point;
}

vector<Line*> Polyline::get_lines() const {
  vector<Line*> lineList;
  for(auto l : lines)
    lineList.push_back(l.line);
  return lineList;
}

Box Polyline::outBox() {
  Point p(0, 0, 0);
  return Box(p, 0, 0, 0);
}

std::vector<KeyPoint*> Polyline::intermidiaryPoints() const {
  std::vector<KeyPoint*> result;
  for(auto l : lines) {
    if (l.direction == LineDirection::DIRECT) {
      if (l.line->final_point != final_point)
        result.push_back(l.line->final_point);
    } else {
      if (l.line->init_point != final_point)
        result.push_back(l.line->init_point);
    }
  }
  return result;
}

}