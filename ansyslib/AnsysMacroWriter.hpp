#pragma once

#include "GeometryList.hpp"
#include <string>

using namespace geomlib;
using namespace std;

namespace ansyslib {

class AnsysMacroWriter {
public:
    AnsysMacroWriter(GeometryList *geometries)
        : geometries(geometries) {}

    string getMacro();

private:
    GeometryList* geometries;
	double greaterX;
	double greaterY;
	double greaterZ;

	KeyPoint* generateGhostPoint();
	Line* generateGhostLine();
};

}