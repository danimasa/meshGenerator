#include "AnsysMeshWriter.hpp"
#include "AnsysMacroPrinter.hpp"

#include <sstream>

namespace ansyslib {

string MeshWriter::getMshFile() {
    // renumbering vertices
    int id = 1;
    for (auto v : mesh->getVertices()) {
        v->setID(id);
        id++;
    }

    AnsysMacroPrinter printer;
    printer.printNodes(mesh->getVertices());
    printer.printElements(mesh->getElements());
    return printer.getString();
}

}