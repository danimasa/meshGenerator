#pragma once

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "stringUtils.hpp"

using namespace std;

static inline int simplePlusConcatenatedLinesCounter(const string &firstLine) {
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