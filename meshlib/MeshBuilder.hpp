#pragma once

#include <memory>
#include "Mesh.hpp"

enum class READING_TYPES {
    NOTHING,
    BLOCK,
    KEYPOINTS,
    LINES
};

class MeshBuilder {
protected:
    shared_ptr<Mesh> mesh;

public:
    MeshBuilder() {
        mesh = shared_ptr<Mesh>(new Mesh());
    }

    virtual void setArraySize(READING_TYPES type, const int size) = 0;

    virtual void AddLine(const shared_ptr<Line> &line) = 0;
    virtual void AddKeypoint(const shared_ptr<KeyPoint> &kp) = 0;

    shared_ptr<Mesh> getMesh() { return mesh; }
};
