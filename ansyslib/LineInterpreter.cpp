#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "LineInterpreter.hpp"
#include "Line.hpp"
#include "stringUtils.hpp"
#include "mathUtils.hpp"

using namespace std;
using namespace geomlib;

namespace ansyslib {

vector<string> splitLine(string &line) {
    trim(line);
    vector<string> lineData;
    boost::split(lineData, line, boost::is_any_of("\t "), boost::token_compress_on);
    return lineData;
}

bool equal(double x, double y) {
    if (double_equals(x, y)) {
        return true;
    }
    return false;
}

geomlib::Geometry* LineInterpreter::interpret(std::string &block) {
    if(block.empty())
        return nullptr;

    trim(block);
    vector<string> lines;
    boost::split(lines, block, boost::is_any_of("\n"), boost::token_compress_on);

    try {
        // Leitura primeira linha
        string line = lines.at(0);
        auto lineData = splitLine(line);

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

        auto init_point = dynamic_cast<KeyPoint*>(geomList->getByID("keypoint", init_point_id));
        auto final_point = dynamic_cast<KeyPoint*>(geomList->getByID("keypoint", final_point_id));

        if (init_point == nullptr || final_point == nullptr) {
            throw std::invalid_argument("init_point and final_point must be defined: " + init_point_str + " - " + final_point_str);
        }

        // Leitura segunda linha
        line = lines.at(1);
        lineData = splitLine(line);

        double lineLength = boost::lexical_cast<double>(lineData[0]);
        double distance = init_point->distance(final_point);
        // Linha Reta
        if (equal(distance, lineLength)) {
            auto straightLine = _factory->createStraightLine(init_point, final_point);
            straightLine->setID(id);
            return straightLine;
        }

        // Criando ponto médio
        double mid_point_x = boost::lexical_cast<double>(lineData[1]);
        double mid_point_y = boost::lexical_cast<double>(lineData[2]);
        double mid_point_z = boost::lexical_cast<double>(lineData[3]);
        auto mid_point = new Point(mid_point_x, mid_point_y, mid_point_z);

        // Leitura terceira linha
        line = lines.at(2);
        lineData = splitLine(line);

        // Criando versor tangente do ponto inicial
        double init_versor_x = boost::lexical_cast<double>(lineData[0]);
        double init_versor_y = boost::lexical_cast<double>(lineData[1]);
        double init_versor_z = boost::lexical_cast<double>(lineData[2]);
        auto init_versor = new Vector(init_versor_x, init_versor_y, init_versor_z);
        
        // Leitura quarta linha
        line = lines.at(3);
        lineData = splitLine(line);

        // Criando versor tangente do ponto final
        double final_versor_x = boost::lexical_cast<double>(lineData[0]);
        double final_versor_y = boost::lexical_cast<double>(lineData[1]);
        double final_versor_z = boost::lexical_cast<double>(lineData[2]);
        auto final_versor = new Vector(final_versor_x, final_versor_y, final_versor_z);

        auto plane = _factory->createPlane(init_point, final_point, mid_point);
        if(plane->contains(init_versor) && plane->contains(final_versor)) {
            // Medir comprimento do arco correspondente e comparar com o comprimento do arquivo
            auto arco = _factory->createArcLine(init_point, final_point, mid_point, init_versor, final_versor);
            auto arcoLength = arco->length();
            if (equal(arco->length(), lineLength)) {
                arco->setID(id);
                return arco;
            }
        }

        // TODO: Algoritmo para diferenciar entre reta e arco
        auto undefinedLine = _factory->createUnspecifiedLine(init_point, final_point, mid_point, init_versor, final_versor);
        undefinedLine->setID(id);
        return undefinedLine;
    } catch( boost::bad_lexical_cast const& ) {
        cout << "Erro: Problema de sintaxe no bloco: " << block << endl;
        return nullptr;
    }
}

}