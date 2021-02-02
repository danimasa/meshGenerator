#include <unordered_map>

#include "GeometryList.hpp"
#include "GeometryFactory.hpp"
#include "MeshFactory.hpp"
#include "MeshShapes.hpp"
#include "MeshShapesGenerator.hpp"
#include "Polyline.hpp"

namespace meshlib {

MeshFactory* MeshFactory::getInstanceImpl(AppParams& params = AppParams{})
{
    static MeshFactory instance { params };
    return &instance;
}

MeshFactory* MeshFactory::getDefaultInstance()
{
    return getInstanceImpl();
}

void MeshFactory::init(AppParams& params)
{
    getInstanceImpl(params);
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

void MeshFactory::m_evenElementsByNumberOfContacts(vector<Line*> &okLines, int nContacts, GeometryList* geomList) {
   auto areas = geomList->getListOf(GeometryType::Area);
   auto geomFactory = GeometryFactory::getDefaultInstance();
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

                vector<Line*> notOkLines;
                for(auto l : eLoop->lines) {
                    auto findOk = std::find(okLines.begin(), okLines.end(), l);
                    if (findOk != okLines.end()) continue;
                    notOkLines.push_back(l);
                }

                geomFactory->lineElementsQtyCorrection(notOkLines, APROX_DIRECTION::BOTH);
            }

            for(auto loopline : eLoop->lines) {
                if (loopline->getLineType() == LineType::Polyline) {
                    auto pLine = dynamic_cast<Polyline*>(loopline);
                    auto pLines = pLine->get_lines();
                    for (auto l : pLines)
                        okLines.push_back(l);
                }
                okLines.push_back(loopline);
            }
        }
   }
}

void MeshFactory::evenElementsInArea(GeometryList* geomList) {
    vector<Line*> okLines;
    for (int i = 4; i >= 0; i--) {
        m_evenElementsByNumberOfContacts(okLines, i, geomList);
    }
}

Vertex* MeshFactory::genVertex(Point *point) const {
    auto vertex = new Vertex(point);
    return vertex;
}

Vertex* MeshFactory::genVertex(double x, double y, double z) const {
    auto vertex = new Vertex(x, y, z);
    return vertex;
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