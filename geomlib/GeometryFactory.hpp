#pragma once

#include "Geometry.hpp"
#include "KeyPoint.hpp"

namespace geomlib {
    class GeometryFactory {
    public:
        static const GeometryFactory* getDefaultInstance();

        KeyPoint* createKeypoint() const;
        KeyPoint* createKeypoint(Point &point) const;

        GeometryFactory(GeometryFactory const&) = delete;
        void operator=(GeometryFactory const&) = delete;
    private:
        GeometryFactory() {}
    };
}