#include "MeshShapes.hpp"
#include <stdexcept>

namespace meshlib {

using namespace std;

vector<MeshShapes::RelativeShapes> MeshShapes::generateShapeList(const QuadArea& area) {
    if(area.someQtdElementsZero() || !area.isEvenElements())
        throw std::invalid_argument("Only even elements in surface boundary permited");

    vector<MeshShapes::RelativeShapes> result;
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
                    result.push_back(MeshShapes::RelativeShapes::PPOO_0);
                } else {
                    s -= 2;
                    result.push_back(MeshShapes::RelativeShapes::PPOO_180);
                }
            }
            // U_90 or U_270
            else {
                if (o > l) {
                    o -= 2;
                    result.push_back(MeshShapes::RelativeShapes::PPOO_90);
                } else {
                    l -= 2;
                    result.push_back(MeshShapes::RelativeShapes::PPOO_270);
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
                    result.push_back(MeshShapes::RelativeShapes::POOO_0);
                } else {
                    o -= 1;
                    n -= 1;
                    result.push_back(MeshShapes::RelativeShapes::POOO_90);
                }
            } else {
                if (o > l) {
                    o -= 1;
                    s -= 1;
                    result.push_back(MeshShapes::RelativeShapes::POOO_180);
                } else {
                    l -= 1;
                    s -= 1;
                    result.push_back(MeshShapes::RelativeShapes::POOO_270);
                }
            }
            continue;
        }

        // Regular mesh
        n -= n - 1;
        s -= s - 1;
        l -= l - 1;
        o -= o - 1;
        result.push_back(MeshShapes::RelativeShapes::OOOO);
    }

    if (n != 1 || s != 1 || l != 1 || o != 1)
        throw std::invalid_argument("insuficient element size for geometry");

    return result;
}

} // namespace meshlib
