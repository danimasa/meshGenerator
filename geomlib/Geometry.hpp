#pragma once

#include <string>
#include <memory>

// Forward Declarations
namespace geomlib {
    class GeometryFactory;
}

namespace geomlib {

    class Geometry {
    public:
        friend class GeometryFactory;

        virtual int getID() const { return ID; }
        virtual void setID(int newID) { ID = newID; }

        virtual std::string getGeometryType() const = 0;

        static int getNextId();

    protected:
        int ID;

        Geometry();
        Geometry(const Geometry &geometry);

    private:
        static int next_id;
    };
}