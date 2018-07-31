#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "KeypointInterpreter.hpp"
#include "KeyPoint.hpp"
#include "stringUtils.hpp"

using namespace std;
using namespace geomlib;

namespace ansyslib {

geomlib::Geometry* KeypointInterpreter::interpret(string &block) {
    if(block.empty())
        return nullptr;

    trim(block);
    vector<string> strs;
    boost::split(strs, block, boost::is_any_of("\t "), boost::token_compress_on);

    try {
        // Get id
        string idstr = strs[0];
        idstr.pop_back();
        int id = boost::lexical_cast<int>(idstr);

        // Get Coordinates
        double x = boost::lexical_cast<double>(strs[1]);
        double y = boost::lexical_cast<double>(strs[2]);
        double z = boost::lexical_cast<double>(strs[3]);
        Point p (x, y, z);
        return _factory->createKeypoint(p);
    } catch( boost::bad_lexical_cast const& ) {
        std::cout << "Erro: Problema de sintaxe na linha: " << block << std::endl;
        return nullptr;
    }
}

}