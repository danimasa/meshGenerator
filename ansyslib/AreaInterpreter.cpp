#include <iostream>
#include <vector>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "AreaInterpreter.hpp"
#include "stringUtils.hpp"

using namespace geomlib;
using namespace std;

namespace ansyslib
{

Line* findLineWithId(string id_str, GeometryList *geomList) {
  id_str.pop_back();
  
  int lineId = boost::lexical_cast<int>(id_str);
  auto corresp_geom = geomList->getByID("line", lineId);

  if (corresp_geom == NULL) {
    stringstream message;
    message << "the line with id " << lineId << " is not in the list of lines";
    throw std::invalid_argument(message.str());
  }

  auto corresp_line = dynamic_cast<Line*>(corresp_geom);
  return corresp_line;
}
  
int AreaInterpreter::getLinesPerObject(const string &firstLine) const {
  string line = firstLine;
  trim(line);
  vector<string> lineData;
  boost::split(lineData, line, boost::is_any_of("\t "), boost::token_compress_on);
  int simpleLinesCount, concatenatedLinesCount;

  string simpleLineStr = lineData[1];
  simpleLineStr.pop_back();
  string concatenatedLineStr = lineData[2];
  concatenatedLineStr.pop_back();

  simpleLinesCount = boost::lexical_cast<int>(simpleLineStr);
  concatenatedLinesCount = boost::lexical_cast<int>(concatenatedLineStr);

  return simpleLinesCount + concatenatedLinesCount + 3;
}

geomlib::Geometry* AreaInterpreter::interpret(string &block) {
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

  if (totalLines <= 2) {
    throw std::invalid_argument("minimal of 3 lines to define an area is necessary.");
  }

  vector<Line*> lineList;
  lineList.reserve(totalLines);

  // Leitura das Linhas
  for (int i=0; i < totalLines; i++) {
    line = lines.at(i + 1);
    auto corresp_line = findLineWithId(line, geomList);
    lineList.push_back(corresp_line);
  }

  line = lines.at(totalLines + 1);
  auto first_line = findLineWithId(line, geomList);
  
  line = lines.at(totalLines + 2);
  auto last_line = findLineWithId(line, geomList);

  auto area = _factory->createArea(lineList, first_line, last_line);
  area->setID(id);

  return area;
}

} // ansyslib
