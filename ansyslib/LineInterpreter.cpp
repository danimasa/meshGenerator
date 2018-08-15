#include <iostream>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "LineInterpreter.hpp"
#include "Line.hpp"
#include "stringUtils.hpp"

using namespace std;
using namespace geomlib;

namespace ansyslib {

geomlib::Geometry* LineInterpreter::interpret(std::string &block) {
    if(block.empty())
        return nullptr;

    trim(block);
    vector<string> lines;
    boost::split(lines, block, boost::is_any_of("\n"), boost::token_compress_on);

    string firstLine = lines.at(0);
    vector<string> lineData;
    boost::split(lineData, firstLine, boost::is_any_of("\t "), boost::token_compress_on);

    try {
        // Get id
        string idstr = lineData[0];
        idstr.pop_back(); // remove ponto extra
        int id = boost::lexical_cast<int>(idstr);

        // Get init and final points
        string init_point_str = lineData[1];
        init_point_str.pop_back();
        string final_point_str = lineData[2];
        final_point_str.pop_back();

        int init_point_id = boost::lexical_cast<int>(init_point_str);
        int final_point_id = boost::lexical_cast<int>(final_point_str);

        return _factory->createLine(init_point_id, final_point_id);
    } catch( boost::bad_lexical_cast const& ) {
        cout << "Erro: Problema de sintaxe no bloco: " << block << endl;
        return nullptr;
    }
}

}