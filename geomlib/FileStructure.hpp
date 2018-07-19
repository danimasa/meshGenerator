#pragma once

#include <list>
#include <memory>
#include "GeometryList.hpp"
#include "Interpreter.hpp"
#include "GeometryBuilder.hpp"

namespace geomlib {

    enum class READING_TYPES {
        NOTHING,
        BLOCK,
        KEYPOINTS,
        LINES,
        AREAS,
        VOLUMES
    };

    class FileStructure {
    protected:
        READING_TYPES reading = READING_TYPES::NOTHING;
        std::list<GeometryBuilder*> builders;
        std::shared_ptr<GeometryList> geometryList;

        FileStructure (std::shared_ptr<GeometryList> geometryList) :
            geometryList(geometryList) {}

        FileStructure() :
            geometryList(std::make_shared<GeometryList>()) {}

    public:
        virtual void readFileLine(std::string line) = 0;

        std::shared_ptr<GeometryList> getGeometryList() { return geometryList; }
        READING_TYPES getReading() { return reading; }
    };
}
