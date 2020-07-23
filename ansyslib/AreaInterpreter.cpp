#include <vector>
#include <stdexcept>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "AreaInterpreter.hpp"
#include "stringUtils.hpp"
#include "LineCounter.hpp"
#include "mathUtils.hpp"
#include "Polyline.hpp"

using namespace geomlib;
using namespace std;

namespace ansyslib
{

Line* findLineWithId(string id_str, GeometryList *geomList) {
  id_str.pop_back();
  
  int lineId = boost::lexical_cast<int>(id_str);
  auto corresp_geom = geomList->getByID(GeometryType::Line, lineId);

  if (corresp_geom == NULL) {
    stringstream message;
    message << "the line with id " << lineId << " is not in the list of lines";
    throw std::invalid_argument(message.str());
  }

  auto corresp_line = dynamic_cast<Line*>(corresp_geom);
  return corresp_line;
}

Area::Loop* findLoopWithLine(Line* line, vector<Area::Loop*> loops) {
  for(auto loop : loops) {
    for(auto l : loop->lines) {
      if (l->getID() == line->getID()) return loop;
    }
  }

  return nullptr;
}

Polyline::Line_in_Polyline nextTouchingLine(const KeyPoint& point, vector<Line*>& remainingLines) {
  int index = -1;
  Line* searchedLine = NULL;
  for(int i = 0; i < remainingLines.size(); i++) {
    auto l = remainingLines[i];
    if(*l->init_point == point || *l->final_point == point) {
      index = i;
      searchedLine = l;
      break;
    }
  }

  if (searchedLine == NULL)
    return Polyline::Line_in_Polyline{ NULL, LineDirection::DIRECT };

  remainingLines.erase(remainingLines.begin() + index);
  
  LineDirection lineDirection = LineDirection::DIRECT;
  if (*searchedLine->final_point == point) lineDirection = LineDirection::INVERSE;
  return Polyline::Line_in_Polyline{ searchedLine, lineDirection };
}

Polyline* findPath(Line* startLine,
  KeyPoint* init_point,
  KeyPoint* final_point,
  vector<Line*> &allLines) {
    KeyPoint searchedPoint = *startLine->final_point;
    if (startLine->final_point == init_point)
      searchedPoint = *startLine->init_point;
    
    vector<Line*> lines;
    lines.push_back(startLine);
    do {
      Polyline::Line_in_Polyline searchedLine = nextTouchingLine(searchedPoint, allLines);
      if(searchedLine.line == NULL) return NULL;
      if(searchedLine.line == startLine) continue;
      lines.push_back(searchedLine.line);

      if(searchedLine.direction == LineDirection::DIRECT) searchedPoint = *searchedLine.line->final_point;
      else searchedPoint = *searchedLine.line->init_point;
    } while(searchedPoint != *final_point);

    auto *factory = GeometryFactory::getDefaultInstance();
    return factory->createPolyline(init_point, final_point, lines);
}

Polyline* findPolyline(UnspecifiedLine* line, vector<Line*> lineList) {
  KeyPoint* startPoint = line->init_point;

  vector<Line*> linesStartingKp;
  vector<Line*> allLinesList;
  for(auto l : lineList) {
    Line* sLine = dynamic_cast<Line*>(l);
    allLinesList.push_back(sLine);
    if (sLine != line && (sLine->init_point == startPoint || sLine->final_point == startPoint))
      linesStartingKp.push_back(sLine);
  }

  for(auto sLine : linesStartingKp) {
    Polyline* polyline = findPath(sLine, line->init_point, line->final_point, allLinesList);
    if (double_equals(polyline->length(), line->length())) {
      return polyline;
    } else {
      delete polyline;
    }
  }
  return NULL;
}

bool AreaInterpreter::belongToCurrentGeometry(const std::string &line) {
  if(accumulatedLinesCount == 0) return true;
  string oldLines = line;
  int dotsNumber = std::count(
    oldLines.begin(),
    oldLines.end(),
    '.');
  return dotsNumber < 3;
}

geomlib::Geometry* AreaInterpreter::interpret() {
  string block = accumulatedLines;

  if(block.empty())
    return nullptr;

  trim(block);
  vector<string> lines;
  boost::split(lines, block, boost::is_any_of("\n"), boost::token_compress_on);

  // Leitura da primeira linha
  string line = lines.at(0);
  auto lineData = splitLine(line);

  // Get id
  string idstr = lineData.at(0);
  idstr.pop_back();
  int id = boost::lexical_cast<int>(idstr);

  // Get lines quantity
  string normal_lines_qtd_str = lineData.at(1);
  normal_lines_qtd_str.pop_back();
  string concatenated_lines_qtd_str = lineData.at(2);
  concatenated_lines_qtd_str.pop_back();

  int normal_lines_qtd = boost::lexical_cast<int>(normal_lines_qtd_str);
  int concatenated_lines_qtd = boost::lexical_cast<int>(concatenated_lines_qtd_str);

  int totalLines = normal_lines_qtd + concatenated_lines_qtd;

  if (normal_lines_qtd <= 2) {
    throw std::invalid_argument("minimal of 3 lines to define an area is necessary.");
  }

  int initial_concatenated_position = lines.size() - concatenated_lines_qtd;
  vector<Area::Loop*> loopList;
  // Leitura dos loops
  int linePos = 1;
  vector<Line*> lineList;

  while(linePos < initial_concatenated_position) {
    line = lines.at(linePos);
    trim(line);
    
    if(line[0] == 'L') {
      if (linePos > 1) {
        auto loop = new Area::Loop(lineList);
        loopList.push_back(loop);
        lineList.clear();
      }
      linePos++;
      continue;
    }

    auto corresp_line = findLineWithId(line, geomList);
    lineList.push_back(corresp_line);
    linePos++;
  }

  auto lineLoop = new Area::Loop(lineList);
  loopList.push_back(lineLoop);

  // Leitura das Linhas
  auto allGeometryLines = geomList->getListOf(GeometryType::Line);
  vector<Line*> allLines;
  for(auto l : allGeometryLines) {
    Line* line = dynamic_cast<Line*>(l);
    allLines.push_back(line);
  }

  // Processar linhas concatenadas
  for(int i = initial_concatenated_position; i < lines.size(); i++) {
    line = lines.at(i);
    auto corresp_line = findLineWithId(line, geomList);
    auto loop = findLoopWithLine(corresp_line, loopList);
    Polyline* polyline;

    if (corresp_line->getLineType() == LineType::UnspecifiedLine) {
      UnspecifiedLine* uLine = dynamic_cast<UnspecifiedLine*>(corresp_line);
      polyline = findPolyline(uLine, allLines);
      polyline->setID(uLine->getID());
      geomList->add(polyline);
      
      auto loopLines = loop->lines;
      auto uLinePos = std::find(loopLines.begin(), loopLines.end(), corresp_line);
      auto pos = std::distance(loopLines.begin(), uLinePos);
      loop->lines[pos] = polyline;
    } else if (corresp_line->getLineType() == LineType::Polyline) {
      polyline = dynamic_cast<Polyline*>(corresp_line);
    } else continue;
  }
  
  auto area = _factory->createArea(loopList);
  area->setID(id);

  return area;
}

} // ansyslib
