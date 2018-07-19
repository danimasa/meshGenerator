#pragma once

#include <list>
#include "GeometryList.hpp"
#include "Interpreter.hpp"

namespace geomlib {

    class GeometryBuilder {
    protected:
        Interpreter* interpreter;
        GeometryList* geometryList;

    public:
        GeometryBuilder(GeometryList* geomList)
            : geometryList(geomList) {}

        virtual std::string getBlockCode() const = 0;
        virtual void readFileLine(const std::string* line) = 0;
    };

}
