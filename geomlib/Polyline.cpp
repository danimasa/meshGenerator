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
  KeyPoint searchedPoint = *init_point;
  KeyPoint finalPoint = *final_point;

  if(!pointInOnlyOneLine(searchedPoint, remainingLines)) {
    searchedPoint = *final_point;
    finalPoint = *init_point;
  }

  do {
    Line_in_Polyline searchedLine = nextTouchingLine(searchedPoint, remainingLines);
    lines.push_back(searchedLine);

    if(searchedLine.direction == LINE_DIRECTION::DIRECT) searchedPoint = *searchedLine.line->final_point;
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

bool Polyline::isPointInLine(const Point &point) {
  return false;
}

Point Polyline::pointInLine(const double position) {
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
    maxLinePosition = litr->line->length() / totalLength;
    finalLinePosition = maxLinePosition;
    while(maxLinePosition < position) {
      sumPassedPosition += maxLinePosition;
      initialLinePosition = maxLinePosition;
      litr++;
      maxLinePosition = sumPassedPosition + ( litr->line->length() / totalLength );
      finalLinePosition = maxLinePosition;
    } 

    Line_in_Polyline lineWIthPoint = *litr;
    double linePosition = (position - initialLinePosition) / (finalLinePosition - initialLinePosition);
    if (lineWIthPoint.direction == LINE_DIRECTION::INVERSE) {
      linePosition = 1 - linePosition;
    }
    auto point = lineWIthPoint.line->pointInLine(linePosition); 
    return point;
}

vector<Line*> Polyline::get_lines() const {
  vector<Line*> lineList;
  for(auto l : lines)
    lineList.push_back(l.line);
  return lineList;
}

}