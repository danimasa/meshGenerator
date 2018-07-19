#include "AnsysFileStructure.hpp"
#include "stringUtils.hpp"

void AnsysFileStructure::readFileLine(std::string line) {
    trim(line);

    // Start reading block
    if (line == "9999.") {
        this->reading = READING_TYPES::BLOCK;
        return;
    }
    // Stop reading block
    else if (line == "-9876.") {
        this->reading = READING_TYPES::NOTHING;
        return;
    }

    // Reading block type
    if (this->reading == READING_TYPES::BLOCK) {
        // Keypoint
        if (line == "10.") {
            this->reading = READING_TYPES::KEYPOINTS;
            this->readingArraySize = true;
            return;
        }

        // Line
        if (line == "20.") {
            this->reading = READING_TYPES::LINES;
            this->readingArraySize = true;
            return;
        }

        // Area
        if (line == "30.") {
            this->reading = READING_TYPES::AREAS;
            this->readingArraySize = true;
            return;
        }
    }

    // Reading ArraySize
    if (this->readingArraySize) {
        int size = std::stoi(line);
        this->builder->setArraySize(this->reading, size);
        this->readingArraySize = false;
        return;
    }

    // Reading Keypoint type
    if (this->reading == READING_TYPES::KEYPOINTS) {
        KeyPoint *point = this->interpreter->interpreteKeypoint(line);
        this->builder->AddKeypoint(shared_ptr<KeyPoint>(point));
        return;
    }

    // Reading Line type
    if (this->reading == READING_TYPES::LINES) {
        Line *rdLine = this->interpreter->interpreteLine(line);
        this->builder->AddLine(shared_ptr<Line>(rdLine));
        return;
    }
}

std::shared_ptr<Geometry> AnsysFileStructure::getGeometry() {
    return this->builder->getGeometry();
}