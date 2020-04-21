#include "MeshWriter.hpp"
#include "GMshPrinter.hpp"

#include <sstream>

namespace gmshlib {

string MeshWriter::getMshFile() {
    // renumering vertices
    int id = 1;
    for (auto v : mesh->getVertices()) {
        v->setID(id);
        id++;
    }

    GMshPrinter printer;
    printer.printNodes(mesh->getVertices());
    printer.printElements(mesh->getElements());
    return printer.getString();
}

}