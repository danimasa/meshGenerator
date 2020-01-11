#include "MeshWriter.hpp"
#include "GMshPrinter.hpp"

#include <sstream>

namespace gmshlib {

string MeshWriter::getMshFile() {
    // renumering vertices
    int id = 1;
    for (auto v : mesh->vertices) {
        v->setID(id);
        id++;
    }

    GMshPrinter printer;
    printer.printNodes(mesh->vertices);
    printer.printElements(mesh->elements);
    return printer.getString();
}

}