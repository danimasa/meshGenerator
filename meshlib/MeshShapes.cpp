#define _USE_MATH_DEFINES
#include "MeshShapes.hpp"
#include "meshlib.hpp"
#include <stdexcept>

namespace meshlib {

using namespace std;
using VShape = MeshShapes::VertexShape;
using RShape = MeshShapes::RelativeShapes;

constexpr VShape O = VShape::ORTHOGONAL;
constexpr VShape P = VShape::POLAR;
constexpr VShape A = VShape::ANY;

struct ElementVertexShapes {
    VShape v1s;
    VShape v2s;
    VShape v3s;
    VShape v4s;
};

VShape getVertexShape(double angle, KeyPoint* kp, bool first) {
    if (kp->type == VertexType::ORTHOGONAL && first) return VShape::ORTHOGONAL;
    if (kp->type == VertexType::POLAR && first) return VShape::POLAR;
    if (angle < (M_PI / 3)) return VShape::ORTHOGONAL;
    if (angle > (2 * M_PI / 3)) return VShape::POLAR;
    return VShape::ANY;
}

RShape computeRalativeShape(ElementVertexShapes& vertexShapes, vector<int> &facesElementsQtd) {
    auto v1s = vertexShapes.v1s;
    auto v2s = vertexShapes.v2s;
    auto v3s = vertexShapes.v3s;
    auto v4s = vertexShapes.v4s;

    int& n = facesElementsQtd[0];
    int& s = facesElementsQtd[1];
    int& l = facesElementsQtd[2];
    int& o = facesElementsQtd[3];

    RShape cShape = RShape::OOOO;

    if (v1s == P && v2s != P && v3s != P && v4s != P) {
        l -= 1;
        n -= 1;
        cShape = RShape::POOO_0;
    }
    else if (v1s != P && v2s == P && v3s != P && v4s != P) {
        o -= 1;
        n -= 1;
        cShape = RShape::POOO_90;
    }
    else if (v1s != P && v2s != P && v3s == P && v4s != P) {
        o -= 1;
        s -= 1;
        cShape = RShape::POOO_180;
    }
    else if (v1s != P && v2s != P && v3s != P && v4s == P) {
        l -= 1;
        s -= 1;
        cShape = RShape::POOO_270;
    }
    else if (v1s == P && v2s == P && v3s != P && v4s != P) {
        n -= 2;
        cShape = RShape::PPOO_0;
    }
    else if (v1s != P && v2s == P && v3s == P && v4s != P) {
        o -= 2;
        cShape = RShape::PPOO_90;
    }
    else if (v1s != P && v2s != P && v3s == P && v4s == P) {
        s -= 2;
        cShape = RShape::PPOO_180;
    }
    else if (v1s == P && v2s != P && v3s != P && v4s == P) {
        l -= 2;
        cShape = RShape::PPOO_270;
    }
    else if (v1s == P && v2s != P && v3s == P && v4s != P) {
        n -= 1;
        s -= 1;
        l -= 1;
        o -= 1;
        cShape = RShape::POPO_0;
    }
    else if (v1s != P && v2s == P && v3s != P && v4s == P) {
        n -= 1;
        s -= 1;
        l -= 1;
        o -= 1;
        cShape = RShape::POPO_90;
    }
    else if (v1s == P && v2s == P && v3s == P && v4s != P) {
        n -= 1;
        o -= 1;
        cShape = RShape::POPP_0;
    }
    else if (v1s != P && v2s == P && v3s == P && v4s == P) {
        s -= 1;
        o -= 1;
        cShape = RShape::POPP_90;
    }
    else if (v1s == P && v2s != P && v3s == P && v4s == P) {
        s -= 1;
        l -= 1;
        cShape = RShape::POPP_180;
    }
    else if (v1s == P && v2s == P && v3s != P && v4s == P) {
        n -= 1;
        l -= 1;
        cShape = RShape::POPP_270;
    }
    else if (v1s == P && v2s == P && v3s == P && v4s == P)
        cShape = RShape::PPPP;
    else if (n != s || l != o)
        cShape = RShape::OOOO_C;
    else {
       n -= n - 1;
       s -= s - 1;
       l -= l - 1;
       o -= o - 1;
       cShape = RShape::OOOO;
    }

    return cShape;
}

vector<RShape> MeshShapes::generateShapeList(const QuadArea& area) {
    if(area.someQtdElementsZero() || !area.isEvenElements())
        throw std::invalid_argument("Only even elements in surface boundary permited");

    vector<RShape> result;
    vector<int> facesElementsQtd { 
        area.north().qtdElements,
        area.south().qtdElements,
        area.east().qtdElements,
        area.west().qtdElements
    };

    int &n = facesElementsQtd[0];
    int &s = facesElementsQtd[1];
    int &l = facesElementsQtd[2];
    int &o = facesElementsQtd[3];

    RShape cShape;

    double angle1 = angleBetweenLines(*area.west().line, *area.south().line);
    double angle2 = angleBetweenLines(*area.south().line, *area.east().line);
    double angle3 = angleBetweenLines(*area.east().line, *area.north().line);
    double angle4 = angleBetweenLines(*area.north().line, *area.west().line);

    auto vertices = area.getVertex();
    auto v1 = vertices[0];
    auto v2 = vertices[1];
    auto v3 = vertices[2];
    auto v4 = vertices[3];

    bool firstV1 = true;
    bool firstV2 = true;
    bool firstV3 = true;
    bool firstV4 = true;

    while(n > 1 && s > 1 && l > 1 && o > 1) {
        auto v1s = getVertexShape(angle1, v1, firstV1);
        auto v2s = getVertexShape(angle2, v2, firstV2);
        auto v3s = getVertexShape(angle3, v3, firstV3);
        auto v4s = getVertexShape(angle4, v4, firstV4);

        // if (v1s == O) angle1 += 10.0 * area.attenuationAngleRatio / (s + o);
        // if (v2s == O) angle2 += 10.0 * area.attenuationAngleRatio / (s + l);
        // if (v3s == O) angle3 += 10.0 * area.attenuationAngleRatio / (l + n);
        // if (v4s == O) angle4 += 10.0 * area.attenuationAngleRatio / (n + o);
        // if (v1s == P) angle1 -= 10.0 * area.attenuationAngleRatio / (s + o);
        // if (v2s == P) angle2 -= 10.0 * area.attenuationAngleRatio / (s + l);
        // if (v3s == P) angle3 -= 10.0 * area.attenuationAngleRatio / (l + n);
        // if (v4s == P) angle4 -= 10.0 * area.attenuationAngleRatio / (n + o);

        // Verify U shape
        int nsDiff = std::abs(n - s);
        int loDiff = std::abs(l - o);
        if(std::abs(nsDiff - loDiff) >= 2.0 && (nsDiff == 0 || loDiff == 0)) {
            // U_0 or U_180
            if(nsDiff > loDiff) {
                if(n > s && v1s != O && v2s != O) {
                    v1s = P;
                    v2s = P;
                } else if (v3s != O && v4s != O) {
                    v3s = P;
                    v4s = P;
                }
            }
            // U_90 or U_270
            else {
                if (o > l && v2s != O && v3s != O) {
                    v2s = P;
                    v3s = P;
                } else if (v1s != O && v4s != O) {
                    v1s = P;
                    v4s = P;
                }
            }
        }

        // Verify L Shape
        else if (nsDiff > 0 && loDiff > 0) {
            if (n > s) {
                if (l > o) {
                    if (v1s != O) v1s = P;
                } else {
                    if (v2s != O) v2s = P;
                }
            } else {
                if (o > l) {
                    if (v3s != O) v3s = P;
                } else {
                    if (v4s != O) v4s = P;
                }
            }
        }

        ElementVertexShapes vShapes {v1s, v2s, v3s, v4s};
        auto cShape = computeRalativeShape(vShapes, facesElementsQtd);
        switch (cShape)
        {
        case RShape::POOO_0:
            firstV1 = false;
            firstV2 = false;
            firstV4 = false;
            break;
        case RShape::POOO_90:
            firstV1 = false;
            firstV2 = false;
            firstV3 = false;
            break;
        case RShape::POOO_180:
            firstV2 = false;
            firstV3 = false;
            firstV4 = false;
            break;
        case RShape::POOO_270:
            firstV1 = false;
            firstV3 = false;
            firstV4 = false;
            break;
        default:
            firstV1 = false;
            firstV2 = false;
            firstV3 = false;
            firstV4 = false;
            break;
        }
        result.push_back(cShape);
    }

    if (n != 1 || s != 1 || l != 1 || o != 1)
        throw std::invalid_argument("insuficient element size for geometry");

    return adjustShapeList(result);
}

std::vector<RShape> MeshShapes::adjustShapeList(std::vector<RShape> &shapes) {
    std::vector<RShape> result;

    std::vector<RShape>::iterator shape = shapes.begin();
    while (shape != shapes.end()) {
        auto nxtShape = std::next(shape);
        if ((*shape == RShape::OOOO_C && *nxtShape == RShape::POOO_0) ||
            (*shape == RShape::POOO_0 && *nxtShape == RShape::OOOO_C)) {
            result.push_back(RShape::AOOOP_0);
            ++shape;
        }
        else if ((*shape == RShape::OOOO_C && *nxtShape == RShape::POOO_90) ||
            (*shape == RShape::POOO_90 && *nxtShape == RShape::OOOO_C)) {
            result.push_back(RShape::AOOOP_90);
            ++shape;
        }
        else if ((*shape == RShape::OOOO_C && *nxtShape == RShape::POOO_180) ||
            (*shape == RShape::POOO_180 && *nxtShape == RShape::OOOO_C)) {
            result.push_back(RShape::AOOOP_180);
            ++shape;
        }
        else if ((*shape == RShape::OOOO_C && *nxtShape == RShape::POOO_270) ||
            (*shape == RShape::POOO_270 && *nxtShape == RShape::OOOO_C)) {
            result.push_back(RShape::AOOOP_270);
            ++shape;
        }
        else result.push_back(*shape);
        ++shape;
    }

    return result;
}

} // namespace meshlib
