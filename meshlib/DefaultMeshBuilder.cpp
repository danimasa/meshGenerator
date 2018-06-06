#include "DefaultMeshBuilder.hpp"

void DefaultMeshBuilder::setArraySize(READING_TYPES type, const int size) {
    switch(type) {
        case READING_TYPES::KEYPOINTS:
            this->mesh.get()->Points.reserve(size);
            break;
        case READING_TYPES::LINES:
            this->mesh.get()->Lines.reserve(size);
            break;
        default:
            break;
    }
}

void DefaultMeshBuilder::AddLine(const shared_ptr<Line> &line) {
    this->mesh.get()->Lines.push_back(line);
}

void DefaultMeshBuilder::AddKeypoint(const shared_ptr<KeyPoint> &kp) {
    this->mesh.get()->Points.push_back(kp);
}