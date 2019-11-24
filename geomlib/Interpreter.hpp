#pragma once

#include "Geometry.hpp"
#include "GeometryFactory.hpp"

namespace geomlib {
    class Interpreter {
    public:
        Interpreter() : 
            _factory(GeometryFactory::getDefaultInstance()),
            accumulatedLinesCount(0),
            accumulatedLines("") {}

        virtual Geometry* interpret() = 0;
        // String code defining the current interpreter
        virtual std::string getBlockCode() const = 0;

        void resetAccumulatedLines();
        // accumulateLine returns true when current line does not belong to current geometry
        bool accumulateLine(const std::string &line);
        void setAccumulatedLines(const std::string &block);

    protected:
        const GeometryFactory* _factory;
        std::string accumulatedLines;
        int accumulatedLinesCount;

        virtual bool belongToCurrentGeometry(const std::string &line) = 0;
    };
}
