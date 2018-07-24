#pragma once

#include "FileStructure.hpp"

using namespace geomlib;

namespace ansyslib {

    class AnsysFileStructure : public FileStructure {
    public:
        AnsysFileStructure(std::shared_ptr<GeometryList> geometryList) :
            FileStructure(geometryList) {}

        void readFileLine(std::string line);
        std::list<GeometryBuilder*> getBuilders();
    };

}

