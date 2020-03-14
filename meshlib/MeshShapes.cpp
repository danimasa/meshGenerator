#include "MeshShapes.hpp"
#include "meshlib.hpp"
#include <stdexcept>

namespace meshlib {

using namespace std;
using VShape = MeshShapes::VertexShape;
using RShape = MeshShapes::RelativeShapes;

VShape getVertexShape(double angle) {
    if (angle < (M_PI / 3)) return VShape::POLAR;
    if (angle > (2 * M_PI / 3)) return VShape::ORTHOGONAL;
    return VShape::ANY;
}

vector<RShape> MeshShapes::shapeListFromElementQtd(const QuadArea& area) {
    if(area.someQtdElementsZero() || !area.isEvenElements())
        throw std::invalid_argument("Only even elements in surface boundary permited");

    vector<RShape> result;
    int n = area.north().qtdElements;
    int s = area.south().qtdElements;
    int l = area.east().qtdElements;
    int o = area.west().qtdElements;

    while(n > 1 && s > 1 && l > 1 && o > 1) {
        // Verify U shape
        int nsDiff = std::abs(n - s);
        int loDiff = std::abs(l - o);
        if(std::abs(nsDiff - loDiff) >= 2.0 && (nsDiff == 0 || loDiff == 0)) {
            // U_0 or U_180
            if(nsDiff > loDiff) {
                if(n > s) {
                    n -= 2;
                    result.push_back(RShape::PPOO_0);
                } else {
                    s -= 2;
                    result.push_back(RShape::PPOO_180);
                }
            }
            // U_90 or U_270
            else {
                if (o > l) {
                    o -= 2;
                    result.push_back(RShape::PPOO_90);
                } else {
                    l -= 2;
                    result.push_back(RShape::PPOO_270);
                }
            }
            continue;
        }

        // Verify L Shape
        if (nsDiff > 0 && loDiff > 0) {
            if (n > s) {
                if (l > o) {
                    l -= 1;
                    n -= 1;
                    result.push_back(RShape::POOO_0);
                } else {
                    o -= 1;
                    n -= 1;
                    result.push_back(RShape::POOO_90);
                }
            } else {
                if (o > l) {
                    o -= 1;
                    s -= 1;
                    result.push_back(RShape::POOO_180);
                } else {
                    l -= 1;
                    s -= 1;
                    result.push_back(RShape::POOO_270);
                }
            }
            continue;
        }

        // Regular mesh
        n -= n - 1;
        s -= s - 1;
        l -= l - 1;
        o -= o - 1;
        result.push_back(RShape::OOOO);
    }

    if (n != 1 || s != 1 || l != 1 || o != 1)
        throw std::invalid_argument("insuficient element size for geometry");

    return result;
}

vector<RShape> MeshShapes::shapeListFromDisposition(const QuadArea& area) {
    vector<RShape> result;
    double angle = 0.0;
    RShape cShape;

    constexpr VShape O = VShape::ORTHOGONAL;
    constexpr VShape P = VShape::POLAR;
    constexpr VShape A = VShape::ANY;

    do {
        // 1 Point
        angle = angleBetweenLines(*area.west().line, *area.south().line);
        auto v1s = getVertexShape(angle);

        // 2 Point
        angle = angleBetweenLines(*area.south().line, *area.east().line);
        auto v2s = getVertexShape(angle);

        // 3 Point
        angle = angleBetweenLines(*area.east().line, *area.north().line);
        auto v3s = getVertexShape(angle);

        // 4 Point
        angle = angleBetweenLines(*area.north().line, *area.west().line);
        auto v4s = getVertexShape(angle);

        if (v1s == P && v2s != P && v3s != P && v4s != P)
            cShape = RShape::POOO_0;
        else if (v1s != P && v2s == P && v3s != P && v4s != P)
            cShape = RShape::POOO_90;
        else if (v1s != P && v2s != P && v3s == P && v4s != P)
            cShape = RShape::POOO_180;
        else if (v1s != P && v2s != P && v3s != P && v4s == P)
            cShape = RShape::POOO_270;
        else if (v1s == P && v2s == P && v3s != P && v4s != P)
            cShape = RShape::PPOO_0;
        else if (v1s != P && v2s == P && v3s == P && v4s != P)
            cShape = RShape::PPOO_90;
        else if (v1s != P && v2s != P && v3s == P && v4s == P)
            cShape = RShape::PPOO_180;
        else if (v1s == P && v2s != P && v3s != P && v4s == P)
            cShape = RShape::PPOO_270;
        else if (v1s == P && v2s != P && v3s == P && v4s != P)
            cShape = RShape::POPO_0;
        else if (v1s != P && v2s == P && v3s != P && v4s == P)
            cShape = RShape::POPO_90;
        else if (v1s == P && v2s == P && v3s == P && v4s != P)
            cShape = RShape::POPP_0;
        else if (v1s != P && v2s == P && v3s == P && v4s == P)
            cShape = RShape::POPP_90;
        else if (v1s == P && v2s != P && v3s == P && v4s == P)
            cShape = RShape::POPP_180;
        else if (v1s == P && v2s == P && v3s != P && v4s == P)
            cShape = RShape::POPP_270;
        else if (v1s == P && v2s == P && v3s == P && v4s == P)
            cShape = RShape::PPPP;
        else
            cShape = RShape::OOOO;

        result.push_back(cShape);
        // update area

    } while (cShape != RShape::OOOO);

    return result;
}

vector<RShape> MeshShapes::mergeShapeLists(
    vector<RShape>& elemQtd,
    vector<RShape>& elemDisp
) {
    vector<RShape> result;
    return result;
}

} // namespace meshlib
