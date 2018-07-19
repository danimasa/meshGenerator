#pragma once

#include "Geometry.hpp"

namespace geomlib {
    class GeometryFactory {
    public:
        static const GeometryFactory* getDefaultInstance();

        GeometryFactory(GeometryFactory const&) = delete;
        void operator=(GeometryFactory const&) = delete;
    private:
        GeometryFactory() {}
    };
}