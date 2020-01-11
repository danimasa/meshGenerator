#include "GMshPrinter.hpp"

namespace gmshlib {

GMshPrinter::GMshPrinter() {
    out << "$MeshFormat" << endl;
    out << "4.1 0 8" << endl;
    out << "$EndMeshFormat" << endl;
}

void GMshPrinter::printNode(const Vertex* node) {
    out << node->x << " " << node->y << " " << node->z << endl;
}

void GMshPrinter::printNodes(const vector<Vertex*> nodes) {
    out << "$Nodes" << endl;
    out << "1 " << nodes.size() << " " << nodes.front()->getID() << " " << nodes.back()->getID() << endl;
    out << "2 1 0 " << nodes.size() << endl;

    for(auto node : nodes) out << node->getID() << endl;
    for(auto node : nodes) printNode(node);

    out << "$EndNodes" << endl;
}

void GMshPrinter::printElement(const Quadrilateral* element) {
    out << element->getID() 
        << " " << element->v1->getID()
        << " " << element->v2->getID()
        << " " << element->v3->getID()
        << " " << element->v4->getID()
        << endl;
}

void GMshPrinter::printElements(const vector<Quadrilateral*> elements) {
    out << "$Elements" << endl;
    out << "1 " << elements.size() << " " << elements.front()->getID()
        << " " << elements.back()->getID() << endl;
    out << "2 1 3 " << elements.size() << endl;

    for(auto elem : elements) printElement(elem);

    out << "$EndElements" << endl;
}

string GMshPrinter::getString() {
    return out.str();
}

}