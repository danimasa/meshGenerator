#pragma once

#include <vector>
#include "GeometryList.hpp"
#include "Interpreter.hpp"

namespace geomlib {

    enum class READING_TYPES {
        NOTHING,
        BLOCK_TYPE,
        ARRAY_LENGTH,
        DATA
    };

    class GeometryBuilder {
    private:
        std::vector<Interpreter*> interpreters;
        GeometryList* geometryList;

        Interpreter* currentInterpreter;
        READING_TYPES reading_state;

    public:
        GeometryBuilder(std::vector<Interpreter*> interpreters)
            : GeometryBuilder(new GeometryList(), interpreters) {}

        GeometryBuilder(GeometryList* geomList, std::vector<Interpreter*> interpreters)
            : geometryList(geomList),
              interpreters(interpreters) {
                  reading_state = READING_TYPES::NOTHING;
              }

        READING_TYPES getReading() const { return reading_state; }
        void readFileLine(const std::string &line);
        GeometryList* getGeometryList() { return geometryList; }
    };

}
