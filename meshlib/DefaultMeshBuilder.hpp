#pragma once

#include "MeshBuilder.hpp"

class DefaultMeshBuilder : public MeshBuilder {
public:
    void setArraySize(READING_TYPES type, const int size);

    void AddLine(const shared_ptr<Line> &line);
    void AddKeypoint(const shared_ptr<KeyPoint> &kp);
};