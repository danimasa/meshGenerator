#include "AnsysMeshWriter.hpp"
#include "AnsysMacroPrinter.hpp"

#include <sstream>

namespace ansyslib {

string MeshWriter::getMshFile() {
    // renumbering vertices
    int id = 1;
    for (auto v : mesh->vertices) {
        v->setID(id);
        id++;
    }

    AnsysMacroPrinter printer;
    printer.printNodes(mesh->vertices);
    printer.printElements(mesh->elements);
    return printer.getString();
}

}