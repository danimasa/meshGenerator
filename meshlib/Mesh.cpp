#include "Mesh.hpp"

namespace meshlib {

void Mesh::addVertices(std::vector<Vertex*> vertices) {
    for(auto vertex : vertices)
        addVertex(vertex);
}

void Mesh::addVertex(Vertex* vertex) {
    if(vertices.find(vertex->getID()) == vertices.end())
        vertices.insert({ vertex->getID(), vertex });
}

void Mesh::addElements(std::vector<Quadrilateral*> elements) {
    for(auto element : elements)
        addElement(element);
}

void Mesh::addElement(Quadrilateral* element) {
    if(elements.find(element->getID()) == elements.end())
        elements.insert({ element->getID(), element });
}

void Mesh::mergeMesh(Mesh *m) {
    addVertices(m->getVertices());
    addElements(m->getElements());
}

std::vector<Vertex*> Mesh::getVertices() const {
    std::vector<Vertex*> vecVertices;
    for(const auto &s : vertices)
        vecVertices.push_back( s.second );
    return vecVertices;
}

std::vector<Quadrilateral*> Mesh::getElements() const {
    std::vector<Quadrilateral*> vecElements;
    for(const auto &s : elements)
        vecElements.push_back( s.second );
    return vecElements;
}

}