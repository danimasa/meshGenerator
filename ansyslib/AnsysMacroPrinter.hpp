#pragma once

#include <string>
#include <sstream>
#include <iostream>

#include "KeyPoint.hpp"
#include "Line.hpp"
#include "Polyline.hpp"

using namespace std;
using namespace geomlib;

namespace ansyslib {

class AnsysMacroPrinter {
public:
	AnsysMacroPrinter() {
		out << "/clear" << endl;
		out << "/prep7" << endl;
		out << endl;
	}

	void printKeypoint(KeyPoint* kp);
	void printSimpleLine(Line* line);
	void printPolyline(Polyline* polyline);
	void printBlankLine();
	void printDeleteLine(Line* line);
	void printDeleteKeypoint(KeyPoint* kp);

	string getString();

private:
	ostringstream out;
};

}