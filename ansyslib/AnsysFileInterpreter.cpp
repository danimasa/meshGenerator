#include "AnsysFileInterpreter.hpp"

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "stringUtils.hpp"

std::unique_ptr<KeyPoint> AnsysFileInterpreter::interpreteKeypoint (std::string line) {
    if(line.empty())
        return nullptr;

    trim(line);
    vector<string> strs;
    boost::split(strs, line, boost::is_any_of("\t "), boost::token_compress_on);
    double x = boost::lexical_cast<double>(strs[1]);
    double y = boost::lexical_cast<double>(strs[2]);
    double z = boost::lexical_cast<double>(strs[3]);
    return unique_ptr<KeyPoint>(new KeyPoint(x, y, z));
}

std::unique_ptr<Line> AnsysFileInterpreter::interpreteLine(std::string line) {
    return unique_ptr<Line> (nullptr);
}