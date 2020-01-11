#pragma once

#include <string>
#include <sstream>
#include <iostream>

#include "Mesh.hpp"

using namespace std;
using namespace meshlib;

namespace gmshlib {

class GMshPrinter {
public:
    GMshPrinter();

    void printNode(const Vertex* node);
    void printNodes(const vector<Vertex*> nodes);
    void printElement(const Quadrilateral* element);
    void printElements(const vector<Quadrilateral*> elements);

    string getString();

private:
    ostringstream out;
};

}