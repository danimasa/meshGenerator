#include "DefaultGeometryBuilder.hpp"

void DefaultGeometryBuilder::setArraySize(READING_TYPES type, const int size) {
    switch(type) {
        case READING_TYPES::KEYPOINTS:
            this->geometry.get()->Points.reserve(size);
            break;
        case READING_TYPES::LINES:
            this->geometry.get()->Lines.reserve(size);
            break;
        default:
            break;
    }
}

void DefaultGeometryBuilder::AddLine(const shared_ptr<Line> &line) {
    this->geometry.get()->Lines.push_back(line);
}

void DefaultGeometryBuilder::AddKeypoint(const shared_ptr<KeyPoint> &kp) {
    this->geometry.get()->Points.push_back(kp);
}