#include "Area.hpp"

#include <algorithm>

namespace geomlib
{

Area::Area(vector<Loop*> loops) :
    Geometry(GeometryType::Area),
    loops(loops) {

    for(auto loop : loops)
        for(auto line : loop->lines)
            line->attachedAreas.push_back(this);
}

Area::~Area() {
    for(auto loop : loops)
        for(auto line : loop->lines)
            line->attachedAreas.erase(
                std::remove(
                    line->attachedAreas.begin(),
                    line->attachedAreas.end(),
                    this
                ), line->attachedAreas.end());
}

void Area::addChild(Area* area) {
    children.push_back(area);
}

} // namespace geomlib