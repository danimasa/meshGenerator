#pragma once

#include "Area.hpp"
#include "GeometryAnalysis.hpp"

namespace processlib {

using namespace geomlib;

class AreaAnalysis : public GeometryAnalysis {
public:
    AreaAnalysis(GeometryList *geomList)
        : GeometryAnalysis(geomList) {}

    void subdivideArea(Area* area);

private:
    void subdivideTriangle(Area* area);
};

}