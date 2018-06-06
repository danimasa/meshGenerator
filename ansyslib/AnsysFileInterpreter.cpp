#include "AnsysFileInterpreter.hpp"

#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "stringUtils.hpp"

KeyPoint* AnsysFileInterpreter::interpreteKeypoint (std::string line) {
    if(line.empty())
        return nullptr;

    trim(line);
    vector<string> strs;
    boost::split(strs, line, boost::is_any_of("\t "), boost::token_compress_on);

    try {
        // Get id
        string idstr = strs[0];
        idstr.pop_back();
        int id = boost::lexical_cast<int>(idstr);

        // Get Coordinates
        double x = boost::lexical_cast<double>(strs[1]);
        double y = boost::lexical_cast<double>(strs[2]);
        double z = boost::lexical_cast<double>(strs[3]);
        return new KeyPoint(id, x, y, z);
    } catch( boost::bad_lexical_cast const& ) {
        std::cout << "Erro: Problema de sintaxe na linha: " << line << std::endl;
        return nullptr;
    }
}

Line* AnsysFileInterpreter::interpreteLine(std::string line) {
    return nullptr;
}