#pragma once

#include "Line.hpp"

namespace geomlib {

class ArcLine : public Line {
public:
    string getLineType() const { return "arc-line"; }
};

}