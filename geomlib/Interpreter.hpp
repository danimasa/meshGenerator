#pragma once

#include "Geometry.hpp"
#include "GeometryFactory.hpp"

namespace geomlib {
    class Interpreter {
    public:
        Interpreter() : _factory(GeometryFactory::getDefaultInstance()) {}

        virtual int getLinesPerObject() const = 0;
        virtual Geometry* interpret(std::string block) = 0;

    private:
        const GeometryFactory* _factory;
    };
}
