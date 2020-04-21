#pragma once
#include <vector>
#include <map>

#include "Vertex.hpp"
#include "Quadrilateral.hpp"

namespace meshlib
{

typedef std::map<int, Vertex*> VertexMap;
typedef std::map<int, Quadrilateral*> ElementMap;

class Mesh {
public:
    void addVertices(std::vector<Vertex*> vertices);
    void addVertex(Vertex* vertex);
    void addElements(std::vector<Quadrilateral*> elements);
    void addElement(Quadrilateral* element);
    void mergeMesh(Mesh *m);

    std::vector<Vertex*> getVertices() const;
    std::vector<Quadrilateral*> getElements() const;

private:
    VertexMap vertices;
    ElementMap elements;
};

} // namespace meshlib
