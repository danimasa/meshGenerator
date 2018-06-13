#pragma once

#include <memory>
#include "Geometry.hpp"

enum class READING_TYPES {
    NOTHING,
    BLOCK,
    KEYPOINTS,
    LINES
};

class GeometryBuilder {
protected:
    shared_ptr<Geometry> geometry;

public:
    GeometryBuilder() {
        geometry = shared_ptr<Geometry>(new Geometry());
    }

    virtual void setArraySize(READING_TYPES type, const int size) = 0;

    virtual void AddLine(const shared_ptr<Line> &line) = 0;
    virtual void AddKeypoint(const shared_ptr<KeyPoint> &kp) = 0;

    shared_ptr<Geometry> getGeometry() { return geometry; }
};
