#include <unordered_map>

#include "GeometryList.hpp"
#include "MeshFactory.hpp"
#include "meshlib.hpp"
#include "MeshShapes.hpp"
#include "MeshShapesGenerator.hpp"
#include "Polyline.hpp"

namespace meshlib {

MeshFactory* MeshFactory::getDefaultInstance()
{
    static MeshFactory instance;
    return &instance;
}

vector<Vertex*> MeshFactory::m_generateLineElements(
    Line* line,
    double pPosition,
    bool direct
) const {
    vector<Vertex*> vList;

    if(line->getLineType() == LineType::Polyline) {
        auto pLine = static_cast<Polyline*>(line);
        auto lines = pLine->getLinesWithDirection();
        for(int i = 0; i < lines.size(); i++) {
            auto l = lines[i];
            double pPosition = line->isPointInLine(*l.line->final_point);
            bool direct = l.direction == LineDirection::DIRECT;
            auto pList = m_generateLineElements(l.line, pPosition, direct);
            vList.insert(vList.end(), pList.begin(), pList.end());
        }
    } else {
        int qtdElements = line->getElementsQty();

        if (qtdElements <= 0.0)
            throw "MeshFactory generateLineElements, necessary set elements quantity first.";

        double step = 1.0 / qtdElements;
        for (int i = 1; i < qtdElements; i++) {
            double pos = (i * step) * pPosition;
            if (!direct) pos = 1.0 - pos;
            auto p = line->pointAtPosition(pos);
            auto v = genVertex(&p);
            vList.push_back(v);
        }
    }

    return vList;
}

void MeshFactory::m_fillLineElementsQty(Line* line) const {
    int qtdElements = ceil(line->length() / elementSize);

    if(line->getLineType() == LineType::Polyline) {
        auto totalLenght = line->length();
        int totalElements = 0;
        auto pLine = static_cast<Polyline*>(line);
        auto lines = pLine->getLinesWithDirection();
        std::vector<std::pair<int, double>> aproxValues;

        for(auto l : lines) {
            double eOriginal = qtdElements * l.line->length() / totalLenght;
            int eApprox = ceil(eOriginal);
            totalElements += eApprox;
            aproxValues.push_back({ eApprox, eOriginal });
        }

        while(totalElements > qtdElements) {
            auto pair = lessErrorApproximation(aproxValues, APROX_DIRECTION::DOWN);
            auto oldPair = aproxValues[pair.first];
            aproxValues[pair.first] = {pair.second, oldPair.second };
            totalElements--;
        }

        double lXPos = 0.0;
        for(int i = 0; i < lines.size(); i++) {
            int elements = aproxValues[i].first;
            auto l = lines[i];
            l.line->setElementsQty(elements);
        }

        // auto pLine = dynamic_cast<Polyline*>(line);
        // auto pLineList = pLine->getLinesWithDirection();
        // auto iPoints = pLine->intermidiaryPoints();
        // int lIndex = 0;
        // int nGenElements = 0;
        // double lastXPos = 0.0;

        // for(auto p : iPoints) {
        //     double pPosition = pLine->isPointInLine(*p);
        //     int nElements = ceil(qtdElements * (pPosition - lastXPos));
        //     auto subLine = pLineList[lIndex];
        //     bool direct = subLine.direction == LineDirection::DIRECT;
        //     auto pList = generateLineVertices(subLine.line, nElements, lastXPos, pPosition, direct);
        //     for(auto v : pList)
        //         vList.push_back(v);

        //     auto v = genVertex(p);
        //     vList.push_back(v);
        //     lIndex++;
        //     nGenElements += nElements;
        //     lastXPos = pPosition;
        // }

        // int nElements = qtdElements - nGenElements;
        // auto subLine = pLineList[lIndex];
        // bool direct = subLine.direction == LineDirection::DIRECT;
        // auto pList = generateLineVertices(subLine.line, nElements, lastXPos, direct);
        // for(auto v : pList)
        //     vList.push_back(v);

    } 

    line->setElementsQty(qtdElements);
}

void MeshFactory::m_evenElementsByNumberOfContacts(vector<Line*> &okLines, int nContacts, GeometryList* geomList) {
   auto areas = geomList->getListOf(GeometryType::Area);
   for (int i = 0; i < areas.size(); i++) {
        auto area = static_cast<Area*>(areas[i]);
        auto eLoop = area->loops[0];
        int nContact = 0;
        int totalElements = 0;
        for(auto loopline : eLoop->lines) {
            auto nAreas = loopline->getAttachedAreas().size();
            totalElements += loopline->getElementsQty();
            if(nAreas > 1) nContact++;
        }
        if (nContact > 4) throw "evenElementsInArea: only allowed quadrilaterals";
        if (nContact == nContacts) {
            if(totalElements % 2 != 0) {
                std::vector<std::pair<int, double>> aproxValues;

                vector<Line*> notOkLines;
                for(auto l : eLoop->lines) {
                    auto findOk = std::find(okLines.begin(), okLines.end(), l);
                    if (findOk != okLines.end()) continue;

                    double eOriginal = l->length() / elementSize;
                    int eApprox = l->getElementsQty();
                    aproxValues.push_back({ eApprox, eOriginal });
                    notOkLines.push_back(l);
                }

                auto pair = lessErrorApproximation(aproxValues, APROX_DIRECTION::BOTH);
                auto editedLine = notOkLines[pair.first];
                editedLine->setElementsQty(pair.second);
            }

            for(auto loopline : eLoop->lines)
                okLines.push_back(loopline);
        }
   }
}

void MeshFactory::evenElementsInArea(GeometryList* geomList) {
    vector<Line*> okLines;
    for (int i = 4; i > 0; i--) {
        m_evenElementsByNumberOfContacts(okLines, i, geomList);
    }
}


void MeshFactory::setElementSize(double elementSize) {
    this->elementSize = elementSize;
}

Vertex* MeshFactory::genVertex(Point *point) const {
    auto vertex = new Vertex(point);
    return vertex;
}

Vertex* MeshFactory::genVertex(double x, double y, double z) const {
    auto vertex = new Vertex(x, y, z);
    return vertex;
}

void MeshFactory::fillLineElementsQty(Line *line) const {
    if (elementSize <= 0.0)
        throw "MeshFactory fillLineElementsQty, necessary set elementSize first";

    return m_fillLineElementsQty(line);
}

vector<Vertex*> MeshFactory::generateLineElements(Line *line) const {
    return m_generateLineElements(line);
}

Mesh* MeshFactory::genSurfaceMesh(QuadArea *area) const {
    auto shapeList = MeshShapes::generateShapeList(*area);
    MeshShapesGenerator gen;
    auto regMesh = gen.genMesh(shapeList, *area);
    auto mesh = transfiniteMapping(regMesh, *area);
    return mesh;
}

QuadArea* MeshFactory::subdivideArea(Area *area) const {
    auto qArea = new QuadArea(area->loops[0]->lines);
    return qArea;
}

}