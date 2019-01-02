#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "AreaInterpreter.hpp"
#include "stringUtils.hpp"

using namespace geomlib;
using namespace std;

namespace ansyslib
{
  
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
  return nullptr;
}

} // ansyslib
